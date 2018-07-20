cmake_minimum_required(VERSION 3.0)

include(CMakeParseArguments)
include(CMakeDependentOption)

find_package(PythonInterp REQUIRED)
if(NOT PYTHONINTERP_FOUND)
  message(FATAL_ERROR "Python interpreter could not be found.")
  return()
endif()

option(ISSOFAPYTHON_USE_VIRTUALENV "Use python virtualenv to install local python packages" OFF)
set(ISSOFAPYTHON_VIRTUALENV_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "If ISSOFAPYTHON_USE_VIRTUALENV is ON: directory to use for python installation (default to build directory)")
cmake_dependent_option(ISSOFAPYTHON_INSTALL_USER "If ISSOFAPYTHON_USE_VIRTUALENV is OFF: use --user option when installing python packages" ON
                       "NOT ISSOFAPYTHON_USE_VIRTUALENV" OFF)
set(ISSOFAPYTHON_INSTALL_PREFIX_DIR ${CMAKE_BINARY_DIR} CACHE PATH "If ISSOFAPYTHON_USE_VIRTUALENV is OFF and ISSOFAPYTHON_INSTALL_USER is OFF: directory to use for python installation (default to build directory)")
set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS "--editable" CACHE STRING "Options given to pip install commands")

option(ISSOFAPYTHON_INSTALL_DURING_CMAKE "Enable installation of python packages during configuration" OFF)
option(ISSOFAPYTHON_INSTALL_DURING_BUILD "Enable installation of python packages during build" ON)
#option(ISSOFAPYTHON_INSTALL_DURING_INSTALL "Enable installation of python packages during install" OFF)

if(ISSOFAPYTHON_USE_VIRTUALENV)
    get_filename_component(PYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE} NAME)
    if(WIN32)
      set(PYTHON_EXECUTABLE_RELPATH "Scripts/${PYTHON_EXECUTABLE_RELPATH}")
    else()
      set(PYTHON_EXECUTABLE_RELPATH "bin/${PYTHON_EXECUTABLE_RELPATH}")
    endif()
    set(ISSOFAPYTHON_EXECUTABLE "${ISSOFAPYTHON_VIRTUALENV_DIR}/${PYTHON_EXECUTABLE_RELPATH}" CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "${ISSOFAPYTHON_PIP_INSTALL_OPTIONS}" CACHE STRING "" FORCE)
    if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE})
        message("Creating python virtualenv in ${ISSOFAPYTHON_VIRTUALENV_DIR} ${ISSOFAPYTHON_EXECUTABLE} a")
        execute_process(COMMAND ${PYTHON_EXECUTABLE} -m virtualenv "${ISSOFAPYTHON_VIRTUALENV_DIR}")
        if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE})
            message(FATAL_ERROR "Failed to create python virtualenv executable ${ISSOFAPYTHON_EXECUTABLE}")
            return()
        endif()
    endif()
elseif(ISSOFAPYTHON_INSTALL_USER)
    set(ISSOFAPYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "--user ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS}" CACHE STRING "" FORCE)
    if(NOT PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE)
        execute_process(COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m site --user-site
                        OUTPUT_VARIABLE PYTHON_USER_SITE_DIR
                        OUTPUT_STRIP_TRAILING_WHITESPACE )
        set(PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE "${PYTHON_USER_SITE_DIR}/easy-install.pth" CACHE "" INTERNAL)
    endif()
endif()

# Add python packages described by a setuptools setup.py file
# Underneath this method will create a cmake target which performs
# an "editable" install of the packages described in the setup.py file
# issofapython_add_package( TARGET <target_name> 
#                           SETUP_FILE <setup_py_file_path>
#                         )
#
function(issofapython_add_package)
    set(ARGV TARGET SETUP_FILE )
    cmake_parse_arguments(ARG "" "" "${ARGV}" ${ARGN})
    
    if(${ARG_TARGET} STREQUAL "")
        message(FATAL_ERROR "No target name specified.")
    endif()
    set(PROJECT_TARGET_PYTHON_PACKAGE ${ARG_TARGET})
    
    set(PROJECT_PYTHON_SETUPTOOLSFILE ${ARG_SETUP_FILE})
    if(NOT EXISTS ${PROJECT_PYTHON_SETUPTOOLSFILE} )
        message(FATAL_ERROR "No python package setup file provided.")
    endif()
    get_filename_component(PROJECT_PYTHON_SETUPTOOLS_DIR ${PROJECT_PYTHON_SETUPTOOLSFILE} DIRECTORY)
    
    if(ISSOFAPYTHON_INSTALL_DURING_CMAKE)
        execute_process(
            COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m pip install ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL} "./"
            WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
        )
    endif()
    
    if(ISSOFAPYTHON_INSTALL_DURING_BUILD)
        get_property( LAST_PACKAGE GLOBAL PROPERTY ISSOFAPYTHON_LAST_ADDED_PACKAGE )
        set(PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${LAST_PACKAGE} )

        if (NOT EXISTS "${PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE}")
            file(WRITE "${PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE}" "")
        endif()
        list( APPEND PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE} )
        list( APPEND PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${PROJECT_PYTHON_SETUPTOOLSFILE} )

        # unfortunately add_custom_command does not support files that are both a dependency and an output of a command
        # ( by maintaining a write counter on the file for example ). It results in a cyclic dependency, hence the
        # intricate global variable ISSOFAPYTHON_LAST_ADDED_PACKAGE which keeps track of the last added target that
        # writes into the easy-install.pth file, so that it can be added as a dependency to prevent concurrent writes
        # on that file.
        set(STAMP ${CMAKE_CURRENT_BINARY_DIR}/stamp-pip-${PROJECT_TARGET_PYTHON_PACKAGE}.txt )
        add_custom_command(
            PRE_BUILD
            OUTPUT ${STAMP}
            COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m pip install ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL} "./"
            COMMAND ${CMAKE_COMMAND} -E touch ${STAMP}
            WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
            DEPENDS ${PROJECT_PYTHON_SETUPTOOLSFILE}
        )

        add_custom_target(${PROJECT_TARGET_PYTHON_PACKAGE} ALL
            DEPENDS ${STAMP} ${PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS}
            )

      set_property(GLOBAL PROPERTY ISSOFAPYTHON_LAST_ADDED_PACKAGE ${PROJECT_TARGET_PYTHON_PACKAGE} )
    endif()
endfunction()
