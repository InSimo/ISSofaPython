/*******************************************************************************
*          Private SOFA components, (c) 2018 InSimo                            *
* CONFIDENTIAL SOURCE CODE. This file is the property of InSimo and should not *
* be redistributed. Commercial use is prohibited without a specific license.   *
*******************************************************************************/

#include "BaseBinding.h"
#include "BaseDataBinding.h"
#include "Exceptions.h"

#include <sofa/core/objectmodel/DDGNode.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/defaulttype/AbstractTypeInfo.h>
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/dataparser/JsonDataParser.h>

#include <pybind11/stl.h>
#include <pybind11/numpy.h>
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



template< typename TArithmeticType >
struct ArithmeticTypeCaster
{
    static long long castToInteger(const TArithmeticType& t)
    {
        long long value(t);
        return value;
    }

    static unsigned long long castToUnsigned(const TArithmeticType& t)
    {
        unsigned long long value(t);
        return value;
    }

    static SReal castToScalar(const TArithmeticType& t)
    {
        SReal value(t);
        return SReal(value);
    }
};

template <>
struct ArithmeticTypeCaster<pybind11::int_>
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
struct ArithmeticTypeCaster<pybind11::float_>
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


template< typename TBoolean >
struct BooleanArithmeticTypeCaster
{
    static long long castToInteger(const TBoolean& b)
    {
        bool value(b);
        return value ? 1 : 0;
    }

    static unsigned long long castToUnsigned(const TBoolean& b)
    {
        bool value(b);
        return value ? 1 : 0;
    }

    static SReal castToScalar(const TBoolean& b)
    {
        bool value(b);
        return value ? 1 : 0;
    }
};

template<>
struct ArithmeticTypeCaster<bool> : public BooleanArithmeticTypeCaster<bool> {};

template<>
struct ArithmeticTypeCaster<pybind11::bool_> : public BooleanArithmeticTypeCaster<pybind11::bool_> {};


template< typename TArithmeticType >
bool setFinalValueFromArithmetic(void* dataPtr, TArithmeticType t, const AbstractMultiValueTypeInfo* typeInfo, std::size_t index = 0)
{
    bool ok = true;

    if (typeInfo && typeInfo->ValidInfo() )
    {
        std::size_t dataSize = typeInfo->finalSize(dataPtr);

        // Special case when we directly assign a python object which wraps a fundamental type 
        // to a BaseData which wraps a container of the same type.
        // The container needs to be resized first before copying the value.
        if (dataSize == 0 && index == 0)
        {
            typeInfo->setFinalSize(dataPtr, 1);
        }

        if (typeInfo->Scalar())
        {            
            typeInfo->setFinalValueScalar(dataPtr, index,
                                          ArithmeticTypeCaster<TArithmeticType>::castToScalar(t) );
        }
        else if (typeInfo->Integer()) 
        {
            typeInfo->setFinalValueInteger(dataPtr, index,
                                           ArithmeticTypeCaster<TArithmeticType>::castToInteger(t));
        }
        else if (typeInfo->Unsigned())
        {
            typeInfo->setFinalValueInteger(dataPtr, index,
                                           ArithmeticTypeCaster<TArithmeticType>::castToUnsigned(t));
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
    if (typeInfo )
    {
        if (typeInfo->String())
        {
            typeInfo->setFinalValueString(dataPtr, index, str);
        }
        else
        {
            // The underlying type is not a string, we try to de-serialize using 
            // istream operator. 
            // If the istream operator is not properly implemented for the underlying type 
            // this can fail silently or give a wrong initialization of the type.
            typeInfo->setDataValueString(dataPtr, str);
        }
    }
    else
    {
        ok = false;
    }

    return ok;
}

template< typename TArithmeticType >
void setDataValueFromPyArithmetic(BaseData* data, TArithmeticType t)
{
    auto dataPtr = data->beginEditVoidPtr();
    bool res = setFinalValueFromArithmetic<TArithmeticType>(dataPtr, t, data->getValueTypeInfo()->SingleValueType() );
    data->endEditVoidPtr();
    if (!res)
    {
        throw SofaDataAttributeError(data, data->getName() );
    }
}

void setDataValueFromString(BaseData* data, const std::string& str)
{
    const bool ok = setFinalValueFromString(data->beginEditVoidPtr(), str, data->getValueTypeInfo()->MultiValueType(), 0);
    data->endEditVoidPtr();
    if (!ok)
    {
        throw SofaDataAttributeError(data, data->getName());
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

template< typename TArithmetic >
bool setFinalValueCastFromPyType(void* dataPtr, const AbstractTypeInfo* typeInfo, pybind11::handle src)
{
    bool ok = true;
    try
    {
        TArithmetic t = src.cast< TArithmetic >();
        ok = setFinalValueFromArithmetic<TArithmetic>(dataPtr,t, typeInfo->MultiValueType());
    }
    catch (const pybind11::cast_error&) // not a TArithmetic 
    {
        ok = false;
    }

    return ok;
}

bool setFinalValueCastFromPyString(void* dataPtr, const AbstractTypeInfo* typeInfo, pybind11::handle src)
{
    bool ok = true;
    try
    {
        std::string str = src.cast< std::string >();
        ok = setFinalValueFromString(dataPtr, str, typeInfo->MultiValueType());
    }
    catch (const pybind11::cast_error&) // not a string 
    {
        ok = false;
    }
    return ok;
}

bool setValuePtrFromPyObjectDispatch(void* dataPtr, const AbstractTypeInfo* typeInfo, pybind11::handle src  )
{
    bool ok = false;

    // note: numpy bool types are not instances of pybind11::bool_, they are taken care of in another statement
    if (pybind11::isinstance<pybind11::bool_>(src)) 
    {
        auto b = pybind11::reinterpret_borrow<pybind11::bool_>(src);
        ok = setFinalValueFromArithmetic<pybind11::bool_>(dataPtr, b, typeInfo->MultiValueType());
    }
    else if (pybind11::isinstance<pybind11::int_>(src))
    {
        auto i = pybind11::reinterpret_borrow<pybind11::int_>(src);
        ok = setFinalValueFromArithmetic<pybind11::int_>(dataPtr, i, typeInfo->MultiValueType());
    }
    else if (pybind11::isinstance<pybind11::float_>(src))
    {
        auto f = pybind11::reinterpret_borrow<pybind11::float_>(src);
        ok = setFinalValueFromArithmetic<pybind11::float_>(dataPtr, f, typeInfo->MultiValueType());
    }
    else if (pybind11::isinstance<pybind11::str>(src))
    {
        auto s = pybind11::reinterpret_borrow<pybind11::str>(src);
        std::string str(s);
        ok = setFinalValueFromString(dataPtr, str, typeInfo->MultiValueType());
    }
    else if (pybind11::isinstance<pybind11::dict>(src))
    {
        auto d = pybind11::reinterpret_borrow<pybind11::dict>(src);
        ok = setStructValuePtrFromPyDict(dataPtr, typeInfo->StructureType(), d);
    }
    else if (pybind11::isinstance<pybind11::list>(src))
    {
        auto l = pybind11::reinterpret_borrow<pybind11::list>(src);
        ok = setValuePtrFromPyContainer<pybind11::list>(dataPtr, typeInfo->ContainerType(), l);
    }
    else if (pybind11::isinstance<pybind11::tuple>(src))
    {
        auto t = pybind11::reinterpret_borrow<pybind11::tuple>(src);
        ok = setValuePtrFromPyContainer<pybind11::tuple>(dataPtr, typeInfo->ContainerType(), t);
    }
    // try pybind11 built in casters from here, if everything else has failed.
    // the expressions below use runtime exceptions if type casting is invalid,
    // so they may have a higher runtime cost.

    // try pybind11 built in cast to c++ double 
    // Anything on the python side that ranges from scalar value to unsigned integer will fall here, 
    // and numpy boolean type as well ( which are distinct from pybind11::bool_ )
    else if (setFinalValueCastFromPyType<double>(dataPtr, typeInfo, src))
    {
        ok = true;
    }
    // try pybind11 buit cast to c++ string 
    else if (setFinalValueCastFromPyString(dataPtr, typeInfo, src))
    {
        ok = true;
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
        throw SofaDataAttributeError(data, "unsupported assignment");
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
            const std::string& attr = data->getName();
            const std::string correctSyntax = attr + ".value for deep copy or " + attr + ".setParent() for data link";
            try
            {
                pyObj.cast<BaseData*>();
                throw SofaDataAttributeError(data, "no implicit BaseData->BaseData conversion, correct syntax: " + correctSyntax);

            }
            catch (pybind11::cast_error&) // not a BaseData
            {
                throw SofaDataAttributeError(data, "no implicit conversion to BaseData, correct syntax: " + correctSyntax);
            }
        }
    }
}


void pushBackDataValueFromPyObject(BaseData* data, pybind11::object pyObj)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        throw SofaDataTypeError(data, "does not hold a container type");
    }

    if (dataContainterTypeInfo->FixedContainerSize())
    {
        throw SofaDataTypeError(data, "does not hold a resizeable container type");
    }

    if (dataContainterTypeInfo->ContainerKind() != sofa::defaulttype::ContainerKindEnum::Array)
    {
        throw SofaDataTypeError(data, "only implemented for array container type");
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
        throw SofaDataTypeError(data, "unsupported assignment");
    }
}

void insertDataValueAtIndexFromPyObject(BaseData* data, std::size_t index, pybind11::object pyObj)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        throw SofaDataTypeError(data, "does not hold a container type");
    }
    if (dataContainterTypeInfo->ContainerKind() != sofa::defaulttype::ContainerKindEnum::Array)
    {
        throw SofaDataTypeError(data, "insert only implemented for array container type");
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
            throw SofaDataValueError(data, "unsupported assignment");
        }
    }
    else
    {
        data->endEditVoidPtr();
        std::string message = data->getName() + " index: " + std::to_string(index) + " size: " + std::to_string(size);
        throw SofaDataIndexError(data, message.c_str());
    }
}

void resizeData(BaseData* data, std::size_t size)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();

    if (!dataContainterTypeInfo)
    {
        // TypeError
        throw SofaDataAttributeError(data,"does not hold a container type");
    }

    if (dataContainterTypeInfo->FixedContainerSize())
    {
        // TypeError
        throw SofaDataAttributeError(data, "does not hold a resizeable container type");
    }

    void* dataPtr = data->beginEditVoidPtr();
    dataContainterTypeInfo->setContainerSize(dataPtr, size);
    data->endEditVoidPtr();
}

pybind11::object getDataPtrValueAsPyObject(const void* dataPtr, const AbstractTypeInfo* typeInfo, const BaseData* data)
{
    if (!typeInfo)
    {
        throw SofaDataAttributeError(data, "unsupported DataTypeInfo");
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
            throw SofaDataAttributeError(data, "unsupported DataTypeInfo");
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
            pybind11::object obj = getDataPtrValueAsPyObject(dataStructMemberPtr, typeInfo, data);
            pyDict[dataStructMemberName.c_str()] = obj;
        }
        return pyDict;
    }
    else if (typeInfo->IsContainer())
    {
        const AbstractContainerTypeInfo* containerTypeInfo = typeInfo->ContainerType();
        //std::size_t containerSize = containerTypeInfo->containerSize(dataPtr);
        pybind11::list pyList;
        auto itData = containerTypeInfo->cbegin(dataPtr);
        while (itData != containerTypeInfo->cend(dataPtr))
        {
            pybind11::object obj = getDataPtrValueAsPyObject(itData.value(), itData.valueType(), data);
            pyList.append(obj);
            ++itData;
        }

        return pyList;

    }
    else if(typeInfo->ValidInfo() &&  typeInfo->IsMultiValue())
    {
        const AbstractMultiValueTypeInfo* mvinfo = typeInfo->MultiValueType();
        if (mvinfo->finalSize(dataPtr)==1 && mvinfo->FixedFinalSize())
        {
            // this type is NOT a vector; return directly the proper native type
            if (mvinfo->String())
            {
                // it's some text
                return pybind11::str(mvinfo->getFinalValueString(dataPtr,0).c_str());
            }
            if (mvinfo->Scalar())
            {
                // it's a SReal
                return pybind11::float_(mvinfo->getFinalValueScalar(dataPtr,0));
            }
            if (mvinfo->Integer())
            {
                // it's some Integer...
                return pybind11::int_((long)mvinfo->getFinalValueInteger(dataPtr,0));
            }
            else
            {
                throw SofaDataAttributeError(data, "unsupported DataTypeInfo");
            }
        }
        else
        {
            // this is a vector; return a python list of the corrsponding type (ints, scalars or strings)
            const int rowWidth = mvinfo->FinalSize();
            const int nbRows = mvinfo->finalSize(dataPtr) / rowWidth;

            pybind11::list rows;
            for (int i=0; i<nbRows; i++)
            {
                pybind11::list row;
                for (int j=0; j<rowWidth; j++)
                {
                    // build each value of the list
                    if (mvinfo->String())
                    {
                        // it's some text
                        row.append(pybind11::str(mvinfo->getFinalValueString(dataPtr, i*rowWidth+j).c_str()));
                    }
                    else if (mvinfo->Scalar())
                    {
                        // it's a SReal
                        row.append(pybind11::float_(mvinfo->getFinalValueScalar(dataPtr, i*rowWidth+j)));
                    }
                    else if (mvinfo->Integer())
                    {
                        // it's some Integer...
                        row.append(pybind11::int_((long)mvinfo->getFinalValueInteger(dataPtr, i*rowWidth+j)));
                    }
                    else
                    {
                        // this type is not yet supported
                        std::cerr << "<ISSofaPython> BaseData_getAttr_value WARNING: unsupported native type=" << data->getValueTypeString().c_str() << " ; returning string value" << std::endl;
                        row.append(pybind11::str(mvinfo->getFinalValueString(dataPtr, i*rowWidth+j).c_str()));
                    }
                }
                rows.append(row);
            }

            return rows;
        }
    }
    else
    {
        throw SofaDataAttributeError(data, "unsupported DataTypeInfo");
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

    pybind11::object obj = getDataPtrValueAsPyObject(data->getValueVoidPtr(), typeInfo, data);
    return obj;
}

pybind11::object getDataValueAtIndex(const BaseData* data, std::size_t index)
{
    const AbstractTypeInfo* dataTypeInfo = data->getValueTypeInfo();
    const AbstractContainerTypeInfo* dataContainterTypeInfo = dataTypeInfo->ContainerType();
    if (!dataContainterTypeInfo)
    {
        throw SofaDataTypeError(data, "does not hold a container type");
    }
    const void* dataPtr = data->getValueVoidPtr();
    const std::size_t size = dataContainterTypeInfo->containerSize(dataPtr);
    pybind11::object pyObj;
    if (index < size)
    {
        const void* valuePtr = dataContainterTypeInfo->getItemValue(dataPtr, index);
        pyObj = getDataPtrValueAsPyObject(valuePtr, dataContainterTypeInfo->getMappedType(), data );
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

pybind11::object getFullPath(BaseData* data)
{
    if (sofa::core::objectmodel::DataFileName* dataFilename = dynamic_cast<sofa::core::objectmodel::DataFileName*>(data))
    {
        return pybind11::str(dataFilename->getFullPath().c_str());
    }

    return pybind11::none();
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
        throw SofaDataAttributeError(data, "unsupported DataTypeInfo for JSON parsing");
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
        throw SofaDataAttributeError(data, "unsupported DataTypeInfo for JSON parsing");
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
        .def("getPath",&getPath) // deprecated, prefer the less verbose "path" property
        .def_property_readonly("path", &getPath)
        .def_property_readonly("fullPath", &getFullPath)
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
