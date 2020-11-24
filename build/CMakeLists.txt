cmake_minimum_required(VERSION 3.0)

project(ISSofaPython)
set( ISSOFAPYTHON_MAJOR_VERSION 0 )
set( ISSOFAPYTHON_MINOR_VERSION 0 )
set( ISSOFAPYTHON_BUILD_VERSION 1 )

set( ISSOFAPYTHON_VERSION ${ISSOFAPYTHON_MAJOR_VERSION}.${ISSOFAPYTHON_MINOR_VERSION}.${ISSOFAPYTHON_BUILD_VERSION} )

set(ISSOFAPYTHON_BUILD_LIB_DIR     ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(ISSOFAPYTHON_BUILD_BIN_DIR     ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(ISSOFAPYTHON_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(ISSOFAPYTHON_BUILD_CMAKE_DIR   ${CMAKE_BINARY_DIR}/ISSofaPython)

set(ISSOFAPYTHON_INSTALL_LIB_DIR     ${CMAKE_INSTALL_LIBDIR}     CACHE PATH "Installation directory for libraries relative to CMAKE_INSTALL_PREFIX variable")
set(ISSOFAPYTHON_INSTALL_BIN_DIR     ${CMAKE_INSTALL_BINDIR}     CACHE PATH "Installation directory for executables relative to CMAKE_INSTALL_PREFIX variable")
set(ISSOFAPYTHON_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files relative to CMAKE_INSTALL_PREFIX variable")
set(ISSOFAPYTHON_INSTALL_CMAKE_DIR   ${ISSOFAPYTHON_INSTALL_LIB_DIR}/cmake/ISSofaPython )

option(ISSOFAPYTHON_WRITE_BYTECODE_RELEASE_ONLY "Allow the generation of cached python bytecode ( .pyc files ) for release builds only" ON)

option(ISSOFAPYTHON_USE_LEGACY_API "Expose some legacy APIs to get the behavior of SofaPython (methods with a 'Legacy' suffix) (can be needed at least temporarily when doing a SofaPython -> ISSofaPython migration of a large codebase)" OFF)

option(ISSOFAPYTHON_CREATE_CHILD_HOOK "Call a hook each time a child node is created from python ( the hook must be manually registered from python: `sofa.HOOKS['createChild'] = some_func` )" OFF)

option(ISSOFAPYTHON_USE_PYBIND11_GIL "Acquire GIL using pybind11::gil_scoped_acquire, otherwise a custom " ON)

get_filename_component(ISSOFAPYTHON_ACTUAL_SOURCE_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

set(PYBIND11_INSTALL ON CACHE BOOL "Install pybind11 header files?" FORCE)
add_subdirectory(${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/pybind11 ${CMAKE_BINARY_DIR}/pybind11 )

# pytbind11 pybind11Tools (build) tree 
configure_file( ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/pybind11/tools/pybind11Tools.cmake 
                ${CMAKE_BINARY_DIR}/pybind11/pybind11Tools.cmake COPYONLY)
# pytbind11 FindPythonLibsNew (build) tree
configure_file( ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/pybind11/tools/FindPythonLibsNew.cmake 
                ${CMAKE_BINARY_DIR}/pybind11/FindPythonLibsNew.cmake COPYONLY)

if( NOT TARGET SofaSimulationCommon )
    find_package(SofaSimulation REQUIRED)
endif()

if(NOT TARGET SofaBaseTopology )
    find_package(SofaBase REQUIRED)
endif()

set(ISSOFAPYTHON_MODULE_DIR ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/module)

set( MODULE_HEADER_FILES
    ${ISSOFAPYTHON_MODULE_DIR}/include/ISSofaPython/GIL.h
    ${ISSOFAPYTHON_MODULE_DIR}/common.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseClassBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseContextBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseDataBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseLinkBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseObjectBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseMappingBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/CollisionModelBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseStateBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseObjectDescriptionBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseNodeBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/ContextBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/DrawToolBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/EventBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/Exceptions.h
    ${ISSOFAPYTHON_MODULE_DIR}/NodeBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/GNodeBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/TopologyBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/BaseMeshTopologyBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/PointSetTopologyModifierBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/TriangleSetTopologyModifierBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/ObjectFactoryBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/PluginManagerBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/SofaSTLBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/PythonController.h
    ${ISSOFAPYTHON_MODULE_DIR}/PythonEvent.h
    ${ISSOFAPYTHON_MODULE_DIR}/VisualParamsBinding.h
    ${ISSOFAPYTHON_MODULE_DIR}/PythonVisitor.h
    ${ISSOFAPYTHON_MODULE_DIR}/ISSofaPythonModule.h
    )

set( MODULE_SOURCE_FILES 
    ${ISSOFAPYTHON_MODULE_DIR}/common.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseClassBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseContextBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseDataBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseLinkBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseObjectBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseMappingBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/CollisionModelBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseStateBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseObjectDescriptionBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseNodeBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/ContextBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/DrawToolBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/EventBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/Exceptions.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/NodeBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/GIL.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/GNodeBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/TopologyBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/BaseMeshTopologyBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/PointSetTopologyModifierBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/TriangleSetTopologyModifierBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/ObjectFactoryBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/PluginManagerBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/PythonController.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/PythonEvent.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/PythonVisitor.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/VisualParamsBinding.cpp
    ${ISSOFAPYTHON_MODULE_DIR}/ISSofaPythonModule.cpp
)

add_library(ISSofaPython SHARED ${MODULE_SOURCE_FILES} ${MODULE_HEADER_FILES} )
target_link_libraries(ISSofaPython PRIVATE pybind11::module SofaSimulationTree SofaBaseTopology ${PYTHON_LIBRARIES})

target_include_directories(ISSofaPython PUBLIC 
                           $<BUILD_INTERFACE:${ISSOFAPYTHON_MODULE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${ISSOFAPYTHON_INSTALL_INCLUDE_DIR}/ISSofaPython>
                           )
target_include_directories(ISSofaPython PRIVATE 
                           $<BUILD_INTERFACE:${ISSOFAPYTHON_MODULE_DIR}>
                           )
set_target_properties(ISSofaPython PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                              SUFFIX "${PYTHON_MODULE_EXTENSION}")

target_compile_definitions( ISSofaPython PRIVATE "SOFA_BUILD_ISSOFAPYTHON")

if (ISSOFAPYTHON_USE_LEGACY_API)
    target_compile_definitions(ISSofaPython PRIVATE "ISSOFAPYTHON_USE_LEGACY_API=1")
endif()

if (ISSOFAPYTHON_CREATE_CHILD_HOOK)
    target_compile_definitions(ISSofaPython PRIVATE "ISSOFAPYTHON_CREATE_CHILD_HOOK=1")
endif()

if(ISSOFAPYTHON_USE_PYBIND11_GIL)
    target_compile_definitions(ISSofaPython PUBLIC "ISSOFAPYTHON_USE_PYBIND11_GIL=1")
endif()

# ISSofaPython generated header file
configure_file( ISSofaPython.h.in
                ${CMAKE_CURRENT_BINARY_DIR}/ISSofaPython/ISSofaPython.h )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/ISSofaPython/ISSofaPython.h
         DESTINATION ${ISSOFAPYTHON_INSTALL_INCLUDE_DIR}/ISSofaPython)

set( PLUGIN_HEADER_FILES 
    ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/initPlugin.h
    ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/PythonSceneLoader.h
)

set( PLUGIN_SOURCE_FILES 
    ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/initPlugin.cpp
    ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/PythonSceneLoader.cpp
)

add_library(ISSofaPythonPlugin SHARED ${PLUGIN_HEADER_FILES} ${PLUGIN_SOURCE_FILES})
target_link_libraries(ISSofaPythonPlugin PRIVATE pybind11::embed SofaSimulationTree ISSofaPython)

if (ISSOFAPYTHON_WRITE_BYTECODE_RELEASE_ONLY)
    target_compile_definitions(ISSofaPythonPlugin PRIVATE $<$<CONFIG:Debug>:ISSOFAPYTHON_DONT_WRITE_BYTECODE>)
    target_compile_definitions(ISSofaPythonPlugin PRIVATE $<$<CONFIG:RelWithDebInfo>:ISSOFAPYTHON_DONT_WRITE_BYTECODE>)
endif()

target_compile_definitions(ISSofaPythonPlugin PRIVATE "SOFA_BUILD_ISSOFAPYTHONPLUGIN")

# On Linux when using a virtualenv, native libraries such as used by numpy fail to load unless the python library is manually loaded
# This code find its SONAME using readelf
message(STATUS "Getting python library SONAME")
if (CMAKE_SYSTEM_NAME MATCHES Linux)
    message(STATUS "PYTHON_LIBRARIES=${PYTHON_LIBRARIES}")
    message(STATUS "PYTHON_LIBRARY=${PYTHON_LIBRARY}")
    message(STATUS "Python_LIBRARIES=${Python_LIBRARIES}")
    message(STATUS "Python_ROOT_DIR=${Python_ROOT_DIR}")
    if (PYTHON_LIBRARIES)
        if (IS_ABSOLUTE "${PYTHON_LIBRARIES}")
            set(PYTHON_LIB_PATH "${PYTHON_LIBRARIES}")
        else()
            # PYTHON_LIBRARIES is not a valid file, try to use find_library
            find_library(PYTHON_LIB_PATH NAMES "${PYTHON_LIBRARIES}" "lib${PYTHON_LIBRARIES}.so" HINTS ${Python_ROOT_DIR} PATH_SUFFIXES lib "lib/${PYTHON_LIBRARIES}")
            message(STATUS "PYTHON_LIB_PATH=${PYTHON_LIB_PATH}")
            if (NOT PYTHON_LIB_PATH)
                message(FATAL_ERROR "Failed to get python library file path")
            endif()
        endif()
        execute_process(COMMAND bash -c "readelf -d \"${PYTHON_LIB_PATH}\" | awk '$2==\"(SONAME)\" { print substr(\$5,2,length(\$5)-2) }'"
            RESULT_VARIABLE SONAME_ERRORCODE
            OUTPUT_VARIABLE PYTHON_LIBRARY_SONAME
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (SONAME_ERRORCODE OR NOT PYTHON_LIBRARY_SONAME)
            message(FATAL_ERROR "Failed to get python library SONAME")
        else()
            target_compile_definitions(ISSofaPythonPlugin PRIVATE "PYTHON_LIBRARY_SONAME=\"${PYTHON_LIBRARY_SONAME}\"")
            message(STATUS "python library SONAME: ${PYTHON_LIBRARY_SONAME}")
        endif()
    endif()
endif()

set_target_properties( ISSofaPythonPlugin PROPERTIES DEBUG_POSTFIX "_d" )

# Add the plugin to SOFA_PRELOAD_ENV so that it is automatically preloaded by runSofa
set_property(TARGET SofaHelper APPEND PROPERTY SOFA_PRELOAD "ISSofaPythonPlugin")
#if(${SOFA_PRELOAD_ENV} MATCHES "ISSofaPythonPlugin")
#else()
#  set(SOFA_PRELOAD_ENV "${SOFA_PRELOAD_ENV};ISSofaPythonPlugin" CACHE STRING "" FORCE)
#endif()
#set(SOFA_PRELOAD_ENV "" CACHE STRING "A list (semicolon separated) of preloaded sofa plugins to write in sofa.env file.")

include(ISSofaPythonSetupTools.cmake)
set(ISSOFAPYTHON_SETUP_PY ${ISSOFAPYTHON_ACTUAL_SOURCE_DIR}/setup.py)
issofapython_add_package(${ISSOFAPYTHON_SETUP_PY})

include(CMakePackageConfigHelpers)

# ISSofaPythonConfigVersion.cmake file (same for build tree and intall)
write_basic_package_version_file(${ISSOFAPYTHON_BUILD_CMAKE_DIR}/ISSofaPythonConfigVersion.cmake
                                 VERSION ${ISSOFAPYTHON_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

install(FILES ${ISSOFAPYTHON_BUILD_CMAKE_DIR}/ISSofaPythonConfigVersion.cmake
        DESTINATION ${ISSOFAPYTHON_INSTALL_CMAKE_DIR})

# ISSofaPythonSetupTools (build)
configure_file( ISSofaPythonSetupTools.cmake
                ${ISSOFAPYTHON_BUILD_CMAKE_DIR}/ISSofaPythonSetupTools.cmake COPYONLY)
        
# ISSofaPythonSetupTools (installed)
install(FILES ISSofaPythonSetupTools.cmake 
        DESTINATION ${ISSOFAPYTHON_BUILD_CMAKE_DIR} )

# ISSofaPythonConfig.cmake (build tree)
set(ISSOFAPYTHON_INCLUDE_DIR "${ISSOFAPYTHON_BUILD_INCLUDE_DIR}")
configure_package_config_file(ISSofaPythonConfig.cmake.in
                              ${ISSOFAPYTHON_BUILD_CMAKE_DIR}/ISSofaPythonConfig.cmake
                              INSTALL_DESTINATION ${ISSOFAPYTHON_BUILD_CMAKE_DIR}
                              INSTALL_PREFIX ${CMAKE_BINARY_DIR}
                              PATH_VARS ISSOFAPYTHON_INCLUDE_DIR)

# ISSofaPythonConfig.cmake (installed)
set(ISSOFAPYTHON_INCLUDE_DIR "${ISSOFAPYTHON_INSTALL_INCLUDE_DIR}")
configure_package_config_file(ISSofaPythonConfig.cmake.in
                              ${ISSOFAPYTHON_BUILD_CMAKE_DIR}/ISSofaPythonConfigInstall.cmake
                              INSTALL_DESTINATION ${ISSOFAPYTHON_INSTALL_CMAKE_DIR}
                              PATH_VARS ISSOFAPYTHON_INCLUDE_DIR)

install(TARGETS ${PROJECT_NAME} ISSofaPythonPlugin
    COMPONENT ISSofaPython_libraries
    EXPORT  ISSofaPythonTargets
    RUNTIME DESTINATION "${ISSOFAPYTHON_INSTALL_BIN_DIR}"
    LIBRARY DESTINATION "${ISSOFAPYTHON_INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${ISSOFAPYTHON_INSTALL_LIB_DIR}"
    PUBLIC_HEADER DESTINATION "${ISSOFAPYTHON_INSTALL_INCLUDE_DIR}/${PROJECT_NAME}"
    )
 
# SofaPythonTargets.cmake (installed)
install(EXPORT ISSofaPythonTargets
        DESTINATION ${ISSOFAPYTHON_INSTALL_CMAKE_DIR} )

# compatibility with find_package
set(ISSOFAPYTHON_INCLUDE_DIR "$<BUILD_INTERFACE:${ISSOFAPYTHON_BUILD_INCLUDE_DIR}>$<INSTALL_INTERFACE:${ISSOFAPYTHON_INSTALL_INCLUDE_DIR}>" CACHE INTERNAL "")
get_property(ISSofaPython_INCLUDE_DIRS TARGET ISSofaPython PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(ISSofaPython_LIBRARIES TARGET ISSofaPython PROPERTY INTERFACE_LINK_LIBRARIES)
set(ISSofaPython_LIBRARIES ISSofaPython ${ISSofaPython_LIBRARIES} CACHE INTERNAL "")
set(ISSofaPython_INCLUDE_DIRS ${ISSofaPython_INCLUDE_DIRS} CACHE INTERNAL "")
