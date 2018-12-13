cmake_minimum_required(VERSION 3.0)

include(CMakeParseArguments)
include(CMakeDependentOption)

find_package(PythonInterp REQUIRED)
if(NOT PYTHONINTERP_FOUND)
  message(FATAL_ERROR "Python interpreter could not be found.")
  return()
endif()

option(ISSOFAPYTHON_USE_VIRTUALENV "Use python virtualenv to install local python packages" ON)
set(ISSOFAPYTHON_VIRTUALENV_DIR "${CMAKE_BINARY_DIR}/python-venv" CACHE PATH "If ISSOFAPYTHON_USE_VIRTUALENV is ON: directory to use for python installation (defaults to <build-directory>/python-venv/)")
set(ISSOFAPYTHON_VIRTUALENV_OPTIONS "--system-site-packages" CACHE STRING "If ISSOFAPYTHON_USE_VIRTUALENV is ON: options given to the virtualenv creation command")
# Track a potential change of the virtualenv options, to re-execute the associated command in such case
if(NOT DEFINED ISSOFAPYTHON_VIRTUALENV_OPTIONS_old OR (ISSOFAPYTHON_VIRTUALENV_OPTIONS STREQUAL ISSOFAPYTHON_VIRTUALENV_OPTIONS_old))
    set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED FALSE)
else()
    set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED TRUE)
endif()
set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_old ${ISSOFAPYTHON_VIRTUALENV_OPTIONS} CACHE INTERNAL "Copy of ISSOFAPYTHON_VIRTUALENV_OPTIONS to track potential modifications")

cmake_dependent_option(ISSOFAPYTHON_INSTALL_USER "If ISSOFAPYTHON_USE_VIRTUALENV is OFF: use --user option when installing python packages" ON
                       "NOT ISSOFAPYTHON_USE_VIRTUALENV" OFF)
set(ISSOFAPYTHON_INSTALL_PREFIX_DIR ${CMAKE_BINARY_DIR} CACHE PATH "If ISSOFAPYTHON_USE_VIRTUALENV is OFF and ISSOFAPYTHON_INSTALL_USER is OFF: directory to use for python installation (default to build directory)")
set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS "--editable" CACHE STRING "Options given to pip install commands")

if(ISSOFAPYTHON_USE_VIRTUALENV)
    get_filename_component(PYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE} NAME)
    if(WIN32)
      set(PYTHON_EXECUTABLE_RELPATH "Scripts/${PYTHON_EXECUTABLE_RELPATH}")
    else()
      set(PYTHON_EXECUTABLE_RELPATH "bin/${PYTHON_EXECUTABLE_RELPATH}")
    endif()
    set(ISSOFAPYTHON_EXECUTABLE "${ISSOFAPYTHON_VIRTUALENV_DIR}/${PYTHON_EXECUTABLE_RELPATH}" CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "${ISSOFAPYTHON_PIP_INSTALL_OPTIONS}" CACHE STRING "" FORCE)
    if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE} OR ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED)
        message("Creating python virtualenv in ${ISSOFAPYTHON_VIRTUALENV_DIR}")
        execute_process(COMMAND ${PYTHON_EXECUTABLE} -m virtualenv ${ISSOFAPYTHON_VIRTUALENV_OPTIONS} "${ISSOFAPYTHON_VIRTUALENV_DIR}")
        if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE})
            message(FATAL_ERROR "Failed to create python virtualenv executable ${ISSOFAPYTHON_EXECUTABLE}")
            return()
        endif()
    endif()
    target_compile_definitions(ISSofaPythonPlugin PRIVATE "ISSOFAPYTHON_USE_VIRTUALENV=\"${ISSOFAPYTHON_USE_VIRTUALENV}\"")
    target_compile_definitions(ISSofaPythonPlugin PRIVATE "ISSOFAPYTHON_VIRTUALENV_DIR=\"${ISSOFAPYTHON_VIRTUALENV_DIR}\"")
elseif(ISSOFAPYTHON_INSTALL_USER)
    set(ISSOFAPYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "--user" ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS} CACHE STRING "" FORCE)
    if(NOT PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE)
        execute_process(COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m site --user-site
                        OUTPUT_VARIABLE PYTHON_USER_SITE_DIR
                        OUTPUT_STRIP_TRAILING_WHITESPACE )
        set(PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE "${PYTHON_USER_SITE_DIR}/easy-install.pth" CACHE "" INTERNAL)
    endif()
endif()

# Add python packages described by a setuptools setup.py file
# Underneath this method will launch the pip install command with the
# appropriate arguments, depending on wether or not a virtualenv is
# installed.
# issofapython_add_package( <setup_py_file_path> )
#
function(issofapython_add_package)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    
    set(PROJECT_PYTHON_SETUPTOOLSFILE ${ARG_UNPARSED_ARGUMENTS})
    if(NOT EXISTS ${PROJECT_PYTHON_SETUPTOOLSFILE} )
        message(FATAL_ERROR "No python package setup file provided.")
    endif()
    get_filename_component(PROJECT_PYTHON_SETUPTOOLS_DIR ${PROJECT_PYTHON_SETUPTOOLSFILE} DIRECTORY)
    
    execute_process(
        COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m pip install ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL} "./"
        WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
    )
endfunction()
