cmake_minimum_required(VERSION 3.0)

include(CMakeParseArguments)

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
    
    find_package(PythonInterp REQUIRED)
    if(NOT PYTHONINTERP_FOUND)
        message(FATAL_ERROR "Python interpreter could not be found.")
        return()
    endif()
    
    if(${ARG_TARGET} STREQUAL "")
        message(FATAL_ERROR "No target name specified.")
    endif()
    set(PROJECT_TARGET_PYHON_PACKAGE ${ARG_TARGET})
    
    set(PROJECT_PYTHON_SETUPTOOLSFILE ${ARG_SETUP_FILE})
    if(NOT EXISTS ${PROJECT_PYTHON_SETUPTOOLSFILE} )
        message(FATAL_ERROR "No python package setup file provided.")
    endif()
    get_filename_component(PROJECT_PYTHON_SETUPTOOLS_DIR ${PROJECT_PYTHON_SETUPTOOLSFILE} DIRECTORY)
    
    
    get_property( LAST_PACKAGE GLOBAL PROPERTY ISSOFAPYTHON_LAST_ADDED_PACKAGE )
    set(PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${LAST_PACKAGE} )

    if(NOT PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE )
        execute_process(COMMAND ${PYTHON_EXECUTABLE} -m site --user-site 
                        OUTPUT_VARIABLE PYTHON_USER_SITE_DIR 
                        OUTPUT_STRIP_TRAILING_WHITESPACE )
        set(PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE "${PYTHON_USER_SITE_DIR}/easy-install.pth" CACHE "" INTERNAL)
    endif()
    
    list( APPEND PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE} )
    list( APPEND PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS ${PROJECT_PYTHON_SETUPTOOLSFILE} )
    
    add_custom_target(${PROJECT_TARGET_PYHON_PACKAGE} ALL
        DEPENDS ${PROJECT_TARGET_PYTHON_PACKAGE_DEPENDS}
    )
    
    # unfortunately add_custom_command does not support files that are both a dependency and an output of a command
    # ( by maintining a write counter on the file for example ). It results in a cyclic dependency, hence the 
    # intricate global variable ISSOFAPYTHON_LAST_ADDED_PACKAGE which keeps track of the last added target that 
    # writes into the easy-install.pth file, so that it can be added as a dependency to prevent concurrent writes 
    # on that file.
    add_custom_command(
        PRE_BUILD
        # OUTPUT ${PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE}
        TARGET ${PROJECT_TARGET_PYHON_PACKAGE}
        COMMAND ${PYTHON_EXECUTABLE} -m pip install --user --editable "./"
        WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
        DEPENDS ${PROJECT_PYTHON_SETUPTOOLSFILE} 
    )
    
    set_property(GLOBAL PROPERTY ISSOFAPYTHON_LAST_ADDED_PACKAGE ${PROJECT_TARGET_PYHON_PACKAGE} )
endfunction()


