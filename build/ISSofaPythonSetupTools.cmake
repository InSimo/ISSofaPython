cmake_minimum_required(VERSION 3.0)

include(CMakeParseArguments)

# Add python packages described by a setuptools setup.py file
# Underneath this method will create a cmake target which performs
# an "editable" install of the packages described in the setup.py file
# issofapython_add_package(<setup_py_file_path>)
#
function(issofapython_add_package)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    find_package(PythonInterp REQUIRED)
    if(NOT PYTHONINTERP_FOUND)
        message(FATAL_ERROR "Python interpreter could not be found.")
        return()
    endif()
    set(PROJECT_PYTHON_SETUPTOOLSFILE ${ARG_UNPARSED_ARGUMENTS})
    if(NOT EXISTS ${PROJECT_PYTHON_SETUPTOOLSFILE} )
        message("No python package setup file provided. Returning")
    endif()
    get_filename_component(PROJECT_PYTHON_SETUPTOOLS_DIR ${PROJECT_PYTHON_SETUPTOOLSFILE} DIRECTORY)
    set(PROJECT_TARGET_PYHON_PACKAGE_UNINSTALL "${PROJECT_NAME}PythonPackageUninstall")
    set(PROJECT_TARGET_PYHON_PACKAGE_INSTALL "${PROJECT_NAME}PythonPackageInstall")

    set(PROJECT_TARGET_PYHON_SETUP ${PROJECT_NAME}PythonSetup)

    add_custom_target(${PROJECT_TARGET_PYHON_SETUP} ALL)
    add_custom_command( TARGET ${PROJECT_TARGET_PYHON_SETUP}
        PRE_BUILD
        COMMAND ${PYTHON_EXECUTABLE} -m pip install --user --editable "./"
        WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
        DEPENDS ${PROJECT_PYTHON_SETUPTOOLS_DIR}
    )
    set_target_properties(${PROJECT_TARGET_PYHON_SETUP} PROPERTIES FOLDER ${PROJECT_NAME}PythonTargets)
endfunction()
