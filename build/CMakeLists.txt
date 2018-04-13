cmake_minimum_required(VERSION 3.0)

project(ISSofaPython)

get_filename_component(ISSOFA_ACTUAL_SOURCE_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

add_subdirectory(${ISSOFA_ACTUAL_SOURCE_DIR}/pybind11 ${CMAKE_CURRENT_BINARY_DIR}/pybind11 )

find_package(SofaSimulation REQUIRED)

set(ISSOFA_MODULE_DIR ${ISSOFA_ACTUAL_SOURCE_DIR}/module)

set( MODULE_HEADER_FILES
    ${ISSOFA_MODULE_DIR}/BaseBinding.h
    ${ISSOFA_MODULE_DIR}/BaseClassBinding.h
    ${ISSOFA_MODULE_DIR}/BaseContextBinding.h
    ${ISSOFA_MODULE_DIR}/BaseDataBinding.h
    ${ISSOFA_MODULE_DIR}/BaseLinkBinding.h
    ${ISSOFA_MODULE_DIR}/BaseObjectBinding.h
    ${ISSOFA_MODULE_DIR}/BaseObjectDescriptionBinding.h
    ${ISSOFA_MODULE_DIR}/BaseNodeBinding.h
    ${ISSOFA_MODULE_DIR}/ContextBinding.h
    ${ISSOFA_MODULE_DIR}/EventBinding.h
    ${ISSOFA_MODULE_DIR}/NodeBinding.h
    ${ISSOFA_MODULE_DIR}/GNodeBinding.h
    ${ISSOFA_MODULE_DIR}/ObjectFactoryBinding.h
    ${ISSOFA_MODULE_DIR}/PluginManagerBinding.h
    ${ISSOFA_MODULE_DIR}/SofaSTLBinding.h
    ${ISSOFA_MODULE_DIR}/PythonController.h
    ${ISSOFA_MODULE_DIR}/PythonEvent.h
    ${ISSOFA_MODULE_DIR}/ISSofaPythonModule.h
    )

set( MODULE_SOURCE_FILES 
    ${ISSOFA_MODULE_DIR}/BaseBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseClassBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseContextBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseDataBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseLinkBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseObjectBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseObjectDescriptionBinding.cpp
    ${ISSOFA_MODULE_DIR}/BaseNodeBinding.cpp
    ${ISSOFA_MODULE_DIR}/ContextBinding.cpp
    ${ISSOFA_MODULE_DIR}/EventBinding.cpp
    ${ISSOFA_MODULE_DIR}/NodeBinding.cpp
    ${ISSOFA_MODULE_DIR}/GNodeBinding.cpp
    ${ISSOFA_MODULE_DIR}/ObjectFactoryBinding.cpp
    ${ISSOFA_MODULE_DIR}/PluginManagerBinding.cpp
    ${ISSOFA_MODULE_DIR}/PythonController.cpp
    ${ISSOFA_MODULE_DIR}/PythonEvent.cpp
    ${ISSOFA_MODULE_DIR}/ISSofaPythonModule.cpp
)

add_library(ISSofaPython SHARED ${MODULE_SOURCE_FILES} ${MODULE_HEADER_FILES} )
target_link_libraries(ISSofaPython PRIVATE pybind11::module SofaSimulationTree ${PYTHON_LIBRARIES})
set_target_properties(ISSofaPython PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                             SUFFIX "${PYTHON_MODULE_EXTENSION}")
set(ISSOFAPYTHON_COMPILER_FLAGS "-DSOFA_BUILD_ISSOFAPYTHON")
# set_target_properties( ${PROJECT_NAME} PROPERTIES VERSION  )
set_target_properties( ISSofaPython PROPERTIES COMPILE_FLAGS "${ISSOFAPYTHON_COMPILER_FLAGS}"  )

set( PLUGIN_HEADER_FILES 
    ${ISSOFA_ACTUAL_SOURCE_DIR}/initPlugin.h
    ${ISSOFA_ACTUAL_SOURCE_DIR}/PythonSceneLoader.h
)

set( PLUGIN_SOURCE_FILES 
    ${ISSOFA_ACTUAL_SOURCE_DIR}/initPlugin.cpp
    ${ISSOFA_ACTUAL_SOURCE_DIR}/PythonSceneLoader.cpp
)

add_library(ISSofaPythonPlugin SHARED ${PLUGIN_HEADER_FILES} ${PLUGIN_SOURCE_FILES})
target_link_libraries(ISSofaPythonPlugin PRIVATE pybind11::embed SofaSimulationTree)
set_target_properties( ISSofaPythonPlugin PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_ISSOFAPYTHONPLUGIN")
set_target_properties( ISSofaPythonPlugin PROPERTIES DEBUG_POSTFIX "_d" )

