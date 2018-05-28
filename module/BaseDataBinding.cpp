/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseDataBinding.h"

#include <sofa/core/objectmodel/DDGNode.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/defaulttype/AbstractTypeInfo.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/dataparser/JsonDataParser.h>

#include <pybind11/stl.h>

#include <stdexcept>

namespace sofa
{
namespace python
{

using sofa::core::objectmodel::DDGNode;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseNode;
using sofa::defaulttype::AbstractTypeInfo;
using sofa::defaulttype::AbstractMultiValueTypeInfo;
using sofa::defaulttype::AbstractContainerTypeInfo;
using sofa::defaulttype::AbstractValueTypeInfo;
using sofa::defaulttype::AbstractStructureTypeInfo;


template< typename PyArithmeticType >
struct PyArithmeticTypeCaster;


template <>
struct PyArithmeticTypeCaster<pybind11::bool_>
{
    static long long castToInteger(const pybind11::bool_& pyBool)
    {
        bool value(pyBool);
        return value ? 1 : 0;
    }

    static unsigned long long castToUnsigned(const pybind11::bool_& pyBool)
    {
        bool value(pyBool);
        return value ? 1 : 0;
    }

    static SReal castToScalar(const pybind11::bool_& pyBool)
    {
        bool value(pyBool);
        return value ? 1 : 0;
    }
};

template <>
struct PyArithmeticTypeCaster<pybind11::int_>
{
    static long long castToInteger(const pybind11::int_& pyInt)
    {
        long long value(pyInt);
        return value;
    }

    static unsigned long long castToUnsigned(const pybind11::int_& pyInt)
    {
        unsigned long long value(pyInt);
        return value;
    }

    static SReal castToScalar(const pybind11::int_& pyInt)
    {
        long long value(pyInt);
        return SReal(value);
    }
};

template <>
struct PyArithmeticTypeCaster<pybind11::float_>
{
    static long long castToInteger(const pybind11::float_& pyFloat)
    {
        double value(pyFloat);
        return (long long)(value);
    }

    static unsigned long long castToUnsigned(const pybind11::float_& pyFloat)
    {
        double value(pyFloat);
        return (unsigned long long)(value);
    }

    static SReal castToScalar(const pybind11::float_& pyFloat)
    {
        return SReal(pyFloat);
    }
};


template< typename PyArithmeticType > 
bool setFinalValueFromPyArithmetic(void* dataPtr, PyArithmeticType pyArithmetic, const AbstractMultiValueTypeInfo* typeInfo, std::size_t index = 0)
{
    bool ok = true;

    if (typeInfo && typeInfo->ValidInfo() )
    {
        if (typeInfo->Scalar())
        {            
            typeInfo->setFinalValueScalar(dataPtr, index,
                PyArithmeticTypeCaster<PyArithmeticType>::castToScalar(pyArithmetic) );
        }
        else if (typeInfo->Integer()) 
        {
            typeInfo->setFinalValueInteger(dataPtr, index,
                PyArithmeticTypeCaster<PyArithmeticType>::castToInteger(pyArithmetic));
        }
        else if (typeInfo->Unsigned())
        {
            typeInfo->setFinalValueInteger(dataPtr, index,
                PyArithmeticTypeCaster<PyArithmeticType>::castToUnsigned(pyArithmetic));
        }
        else
        {
            ok=false;
        }
    }
    else
    {
        ok = false;
    }

    return ok;
}

bool setFinalValueFromString(void* dataPtr, const std::string& str, const AbstractMultiValueTypeInfo* typeInfo, std::size_t index = 0)
{
    bool ok = true;
    if (typeInfo && typeInfo->String())
    {
        typeInfo->setFinalValueString(dataPtr, index, str);
    }
    else
    {
        ok = false;
    }

    return ok;
}

template< typename PyArithmeticType >
void setDataValueFromPyArithmetic(BaseData* data, PyArithmeticType pyArithmetic)
{
    auto dataPtr = data->beginEditVoidPtr();
    bool res = setFinalValueFromPyArithmetic<PyArithmeticType>(dataPtr, pyArithmetic, data->getValueTypeInfo()->SingleValueType() );
    data->endEditVoidPtr();
    if (!res)
    {
        // type mismatch or too long list
        throw std::invalid_argument("type mismatch");
    }
}

void setDataValueFromString(BaseData* data, const std::string& str)
{
    const bool ok = setFinalValueFromString(data->beginEditVoidPtr(), str, data->getValueTypeInfo()->MultiValueType(), 0);
    data->endEditVoidPtr();
    if (!ok)
    {
        // type mismatch or too long list
        throw std::invalid_argument("type mismatch");
    }
}

bool setValuePtrFromPyObjectDispatch(void* dataPtr, const AbstractTypeInfo* typeInfo, pybind11::handle src );

template< typename PyContainerType >
bool setValuePtrFromPyContainer(void* dataPtr, const AbstractContainerTypeInfo* containerInfo, PyContainerType pyContainer)
{
    bool ok = (containerInfo != nullptr );
    std::size_t pyContainerSize = pyContainer.size();

    if (containerInfo)
    {
        containerInfo->setContainerSize(dataPtr, pyContainerSize); // try to resize, no-op if fixed size;
        std::size_t containerSize = containerInfo->containerSize(dataPtr);
        std::size_t copySize      = std::min(containerSize, pyContainerSize); // maybe we want to catch these kinds of error as well...

        auto itData = containerInfo->begin(dataPtr);
        auto itPyContainer = pyContainer.begin();
        std::size_t copyIndex = 0;

        while (copyIndex < copySize)
        {
            if ( !setValuePtrFromPyObjectDispatch(itData.value(), itData.valueType(), *itPyContainer ) )
            {
                ok = false;
                break;
            }
            ++itPyContainer;
            ++itData;
            ++copyIndex;
        }
    }
    return ok;
}


bool setStructValuePtrFromPyDict(void* dataPtr, const AbstractStructureTypeInfo* structTypeInfo, pybind11::dict pyDict)
{
    bool ok = (structTypeInfo != nullptr);

    if (structTypeInfo != nullptr)
    {
        const std::size_t structSize = structTypeInfo->structSize();

        for (std::size_t i=0; i< structSize; ++i)
        {
            std::string dataStructMemberName = structTypeInfo->getMemberName(dataPtr, i);

            if ( pyDict.contains(dataStructMemberName.c_str()) )
            {
                const AbstractTypeInfo* typeInfo = structTypeInfo->getMemberTypeForIndex(i);
                void* dataStructMemberPtr = structTypeInfo->editMemberValue(dataPtr, i);
                if (!setValuePtrFromPyObjectDispatch(dataStructMemberPtr, typeInfo, pyDict[dataStructMemberName.c_str()]))
                {
                    ok = false;
                    break;
                }
            }
            // TODO: maybe issue a warning here. It probably means we mistyped something.
        }
    }
    return ok;

}

bool setValuePtrFromPyObjectDispatch(void* dataPtr, const AbstractTypeInfo* typeInfo, pybind11::handle src  )
{
    bool ok = true;
    if (pybind11::isinstance<pybind11::bool_>(src))
    {
        auto b = pybind11::reinterpret_borrow<pybind11::bool_>(src);
        ok = setFinalValueFromPyArithmetic<pybind11::bool_>(dataPtr, b, typeInfo->MultiValueType() );
    }
    else if (pybind11::isinstance<pybind11::int_>(src))
    {
        auto i = pybind11::reinterpret_borrow<pybind11::int_>(src);
        ok = setFinalValueFromPyArithmetic<pybind11::int_>(dataPtr, i, typeInfo->MultiValueType() );
    }
    else if (pybind11::isinstance<pybind11::float_>(src))
    {
        auto f = pybind11::reinterpret_borrow<pybind11::float_>(src);
        ok = setFinalValueFromPyArithmetic<pybind11::float_>(dataPtr, f, typeInfo->MultiValueType() );
    }
    else if (pybind11::isinstance<pybind11::str>(src))
    {
        auto s = pybind11::reinterpret_borrow<pybind11::str>(src);
        std::string str(s);
        ok = setFinalValueFromString(dataPtr, str, typeInfo->MultiValueType() );
    }
    else if (pybind11::isinstance<pybind11::dict>(src))
    {
        auto d = pybind11::reinterpret_borrow<pybind11::dict>(src);
        ok = setStructValuePtrFromPyDict(dataPtr, typeInfo->StructureType(), d);
    }
    else if (pybind11::isinstance<pybind11::list>(src))
    {
        auto l = pybind11::reinterpret_borrow<pybind11::list>(src);
        ok = setValuePtrFromPyContainer<pybind11::list>(dataPtr,typeInfo->ContainerType(), l  );
    }
    else if (pybind11::isinstance<pybind11::tuple>(src))
    {
        auto t = pybind11::reinterpret_borrow<pybind11::tuple>(src);
        ok = setValuePtrFromPyContainer<pybind11::tuple>(dataPtr,typeInfo->ContainerType(), t );
    }

    return ok;
}

template <typename PyContainerType >
void setDataValueFromPyContainer(BaseData* data, PyContainerType pyContainer)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    void * dataPtr = data->beginEditVoidPtr();
    const bool ok = setValuePtrFromPyContainer<PyContainerType>(dataPtr, data->getValueTypeInfo()->ContainerType(), pyContainer);
    data->endEditVoidPtr();
    if (!ok)
    {
        throw std::invalid_argument("Unsupported argument");
    }
}

void setDataValueFromPyObject(BaseData* data, pybind11::object pyObj)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    
    if (pybind11::isinstance<pybind11::str>(pyObj))
    {
        auto s = pybind11::reinterpret_borrow<pybind11::str>(pyObj);
        std::string str(s);
        void * dataPtr = data->beginEditVoidPtr();
        dataTypeInfo->setDataValueString(dataPtr, str);
        data->endEditVoidPtr();
    }
    else
    {
        void * dataPtr = data->beginEditVoidPtr();
        const bool ok = setValuePtrFromPyObjectDispatch(dataPtr, dataTypeInfo, pyObj);
        data->endEditVoidPtr();
        if (!ok)
        {
            throw std::invalid_argument("Unsupported argument");
        }
    }
}


void pushBackDataValueFromPyObject(BaseData* data, pybind11::object pyObj)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        throw std::invalid_argument("Data does not hold a container type");
    }

    if (dataContainterTypeInfo->FixedContainerSize())
    {
        throw std::invalid_argument("Data does not hold a resizable container type");
    }

    if (dataContainterTypeInfo->ContainerKind() != sofa::defaulttype::ContainerKindEnum::Array)
    {
        throw std::invalid_argument("Only implemented for Array container kind.");
    }

    void* dataPtr = data->beginEditVoidPtr();
    const std::size_t previousSize = dataContainterTypeInfo->containerSize(dataPtr);
    dataContainterTypeInfo->setContainerSize(dataPtr, previousSize+1);
    void* valuePtr = dataContainterTypeInfo->editItemValue(dataPtr, previousSize);
    const bool ok = setValuePtrFromPyObjectDispatch(valuePtr, dataContainterTypeInfo->getMappedType(), pyObj);
    if (!ok)
    {
        dataContainterTypeInfo->setContainerSize(dataPtr, previousSize);
    }
    data->endEditVoidPtr();
    if (!ok)
    {
        throw std::invalid_argument("Unsupported argument");
    }
}

void insertDataValueAtIndexFromPyObject(BaseData* data, std::size_t index, pybind11::object pyObj)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        throw std::invalid_argument("Data does not hold a container type");
    }
    if (dataContainterTypeInfo->ContainerKind() != sofa::defaulttype::ContainerKindEnum::Array)
    {
        throw std::invalid_argument("Currently only implemented for Array container kind.");
    }
    void* dataPtr = data->beginEditVoidPtr();
    const std::size_t size = dataContainterTypeInfo->containerSize(dataPtr);
    if (index < size)
    {
        void* valuePtr = dataContainterTypeInfo->editItemValue(dataPtr, index);
        const bool ok = setValuePtrFromPyObjectDispatch(valuePtr, dataContainterTypeInfo->getMappedType(), pyObj);
        data->endEditVoidPtr();
        if (!ok)
        {
            throw std::invalid_argument("Unsupported argument");
        }
    }
    else
    {
        data->endEditVoidPtr();
        std::string message = "index: " + std::to_string(index) + " size: " + std::to_string(size);
        throw pybind11::index_error(message.c_str());
    }
}

void resizeData(BaseData* data, std::size_t size)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        throw std::invalid_argument("Data does not hold a container type");
    }

    if (dataContainterTypeInfo->FixedContainerSize())
    {
        throw std::invalid_argument("Data does not hold a resizable container type");
    }

    void* dataPtr = data->beginEditVoidPtr();
    dataContainterTypeInfo->setContainerSize(dataPtr, size);
    data->endEditVoidPtr();
}

pybind11::object getDataPtrValueAsPyObject(const void* dataPtr, const AbstractTypeInfo* typeInfo)
{
    if (!typeInfo)
    {
        throw std::invalid_argument("Unsupported DataTypeInfo");
        return pybind11::object();
    }
    if (typeInfo->IsSingleValue())
    {
        const AbstractValueTypeInfo* singleTypeInfo = typeInfo->SingleValueType();
        if (singleTypeInfo->Integer())
        {
            pybind11::int_ pyInt(singleTypeInfo->getFinalValueInteger(dataPtr, 0));
            return pyInt;
        }
        else if (singleTypeInfo->Unsigned())
        {
            pybind11::int_ pyInt((unsigned long long)(singleTypeInfo->getFinalValueInteger(dataPtr, 0)));
            return pyInt;
        }
        else if (singleTypeInfo->Scalar())
        {
            pybind11::float_ pyFloat(singleTypeInfo->getFinalValueScalar(dataPtr, 0));
            return pyFloat;
        }
        else if (singleTypeInfo->String())
        {
            pybind11::str pyString(singleTypeInfo->getFinalValueString(dataPtr, 0));
            return pyString;
        }
        else
        {
            throw std::invalid_argument("unsupported DataTypeInfo");
            return pybind11::object();
        }
    }
    else if (typeInfo->IsStructure())
    {
        const AbstractStructureTypeInfo* structTypeInfo = typeInfo->StructureType();
        const std::size_t structSize = structTypeInfo->structSize();

        pybind11::dict pyDict;
        for (std::size_t i= 0; i<structSize; ++i)
        {
            const AbstractTypeInfo* typeInfo = structTypeInfo->getMemberTypeForIndex(i);
            std::string dataStructMemberName = structTypeInfo->getMemberName(dataPtr, i);
            const void* dataStructMemberPtr = structTypeInfo->getMemberValue(dataPtr, i);
            pybind11::object obj = getDataPtrValueAsPyObject(dataStructMemberPtr, typeInfo);
            pyDict[dataStructMemberName.c_str()] = obj;
        }
        return pyDict;
    }
    else if (typeInfo->IsContainer())
    {
        const AbstractContainerTypeInfo* containerTypeInfo = typeInfo->ContainerType();
        std::size_t containerSize = containerTypeInfo->containerSize(dataPtr);
        if (containerTypeInfo->FixedContainerSize())
        {
            pybind11::tuple pyTuple(containerSize);
            auto itData = containerTypeInfo->cbegin(dataPtr);
            std::size_t index=0;
            while (itData != containerTypeInfo->cend(dataPtr))
            {
                pybind11::object obj = getDataPtrValueAsPyObject(itData.value(), itData.valueType());
                pyTuple[index] = obj;
                ++itData;
                ++index;
            }

            return pyTuple;
        }
        else
        {
            pybind11::list pyList;
            auto itData = containerTypeInfo->cbegin(dataPtr);
            while (itData != containerTypeInfo->cend(dataPtr))
            {
                pybind11::object obj = getDataPtrValueAsPyObject(itData.value(), itData.valueType());
                pyList.append(obj);
                ++itData;
            }

            return pyList;
        }
    }
    else
    {
        throw std::invalid_argument("unsupported DataTypeInfo");
        return pybind11::object();
    }
}

pybind11::object getDataValueAsPyObject(const BaseData* data)
{
    auto typeInfo =  data->getValueTypeInfo();

    if (typeInfo && !typeInfo->ValidInfo()) // fallback mode, expose a string
    {
        std::string str = data->getValueString();
        pybind11::str pyStr(str);
        return pyStr;
    }

    pybind11::object obj = getDataPtrValueAsPyObject(data->getValueVoidPtr(), typeInfo);
    return obj;
}

pybind11::object getDataValueAtIndex(const BaseData* data, std::size_t index)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();
    if (!dataContainterTypeInfo)
    {
        throw std::invalid_argument("Data does not hold a container type");
    }
    const void* dataPtr = data->getValueVoidPtr();
    const std::size_t size = dataContainterTypeInfo->containerSize(dataPtr);
    pybind11::object pyObj;
    if (index < size)
    {
        const void* valuePtr = dataContainterTypeInfo->getItemValue(dataPtr, index);
        pyObj = getDataPtrValueAsPyObject(valuePtr, dataContainterTypeInfo->getMappedType() );
    }
    else
    {
        std::string message = "index: " + std::to_string(index) + " size: " + std::to_string(size);
        throw pybind11::index_error(message.c_str());
    }
    return pyObj;
}

std::string getPath(const BaseData* data)
{
    std::string path;
    if (data->getOwner())
    {
        if (BaseObject* obj = BaseObject::DynamicCast(data->getOwner()))
        {
            if (BaseNode* node = BaseNode::DynamicCast(obj->getContext()))
            {
                path += "@"+node->getPathName()+"/"+obj->getName()+"."+data->getName();
            }
        }
        else if (BaseContext* ctx = BaseContext::DynamicCast(data->getOwner()))
        {
            if (BaseNode* node = BaseNode::DynamicCast(ctx))
            {
                path += "@"+node->getPathName()+"."+data->getName();
            }
        }
    }
    return path;
}

pybind11::object getDataJsonAsPyObject(const BaseData* data)
{
    // depending on the data type, we return the good python type (int, float, sting, array, ...)

    const AbstractTypeInfo *typeinfo = data->getValueTypeInfo();
    const void* valueVoidPtr = data->getValueVoidPtr();
    std::string output;
    if (typeinfo->ValidInfo())
    {
        sofa::core::dataparser::JsonDataParser JsonParser(data->getName());
        JsonParser.fromData(output, valueVoidPtr, typeinfo);
    }
    else
    {
        throw std::invalid_argument("Unsupported DataTypeInfo for JSON parsing");
        output = "{}";
    }
    pybind11::str pyStr(output);
    return pyStr;
}

void setDataJsonFromPyObject(BaseData* data, const std::string& json)
{
    const AbstractTypeInfo *typeinfo = data->getValueTypeInfo();
    if (typeinfo->ValidInfo())
    {
        sofa::core::dataparser::JsonDataParser JsonParser(data->getName());
        void* editVoidPtr = data->beginEditVoidPtr();
        JsonParser.toData(json, editVoidPtr, typeinfo);
        data->endEditVoidPtr();
    }
    else
    {
        throw std::invalid_argument("Unsupported DataTypeInfo for JSON parsing");
    }
}

void initBindingBaseData(pybind11::module& m)
{
    pybind11::class_<DDGNode>(m, "DDGNode");

    pybind11::class_<BaseData, DDGNode>(m, "BaseData")
        .def("setName", &BaseData::setName)
        .def("getName", &BaseData::getName, pybind11::return_value_policy::copy)
        .def_property("name", &BaseData::getName, &BaseData::setName, pybind11::return_value_policy::copy)
        .def("setValue", &setDataValueFromPyObject)
        .def("resize", &resizeData)
        .def("push_back", &pushBackDataValueFromPyObject)
        .def("insert", &insertDataValueAtIndexFromPyObject)
        .def("getValue", &getDataValueAsPyObject)
        .def("at",&getDataValueAtIndex)
        .def_property("value", &getDataValueAsPyObject, &setDataValueFromPyObject)
        .def_property("json", &getDataJsonAsPyObject, &setDataJsonFromPyObject)
        .def("getValueString",&BaseData::getValueString)
        .def("getPath",&getPath)
        .def("getParent",&BaseData::getParent)
        .def("setParent", pybind11::overload_cast<const std::string&>(&BaseData::setParent) )
        .def("setParent", pybind11::overload_cast<BaseData*,const std::string&>(&BaseData::setParent), 
                          pybind11::arg("parent"),  pybind11::arg("path") = std::string() )
        .def("setOwner", &BaseData::setOwner)
        .def("getOwner", &BaseData::getOwner)
        ;
}

}

}
