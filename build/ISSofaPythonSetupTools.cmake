cmake_minimum_required(VERSION 3.0)

# Using an include guard, even though this is not mandatory, to avoid
# duplicating the install commands for the python environment setup
get_property(ISSOFAPYTHON_PY_SETUP_DONE GLOBAL PROPERTY ISSOFAPYTHON_PY_SETUP_DONE)
if (ISSOFAPYTHON_PY_SETUP_DONE)
    return()
else()
    message(STATUS "Doing the ISSofaPython python setup")
endif()
set_property(GLOBAL PROPERTY ISSOFAPYTHON_PY_SETUP_DONE TRUE)

include(CMakeParseArguments)
include(CMakeDependentOption)

find_package(PythonInterp REQUIRED)
if(NOT PYTHONINTERP_FOUND)
  message(FATAL_ERROR "Python interpreter could not be found.")
  return()
endif()

option(ISSOFAPYTHON_USE_VIRTUALENV "Use a local python environment (fully standalone when possible, else a virtualenv) for python packages instead of 'polluting' the system installation" ON)
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
    if(EMBEDDED_PYTHON AND (DEFINED ENV{PYTHONHOME} OR DEFINED ENV{PYTHONPATH}))
        # When embedding a full python distribution, if a PYTHONHOME and/or PYTHONPATH env var
        # is defined, the packages we need may not always be installed at the correct place,
        # or pip could detect that some of them are already installed and not install them in our
        # local environment
        if(DEFINED ENV{PYTHONHOME})
            message(FATAL_ERROR "A PYTHONHOME env var was detected (with value '$ENV{PYTHONHOME}'). Please remove any PYTHONHOME or PYTHONPATH env var first !")
        endif()
        if(DEFINED ENV{PYTHONPATH})
            message(FATAL_ERROR "A PYTHONPATH env var was detected (with value '$ENV{PYTHONPATH}'). Please remove any PYTHONHOME or PYTHONPATH env var first !")
        endif()
    endif()

    # We set a cached var with a generic name for the python environment folder name,
    # so that it can also be used in other generic modules
    set(PYTHON_ENV_DIRNAME "python-venv" CACHE PATH "If ISSOFAPYTHON_USE_VIRTUALENV is ON: name of the folder containing the local python environment (fully standalone when possible, else a virtualenv)")

    set(ISSOFAPYTHON_VIRTUALENV_DIR "${CMAKE_BINARY_DIR}/${PYTHON_ENV_DIRNAME}" CACHE INTERNAL "")
    set(ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR "\${CMAKE_INSTALL_PREFIX}/${DEFAULT_BIN_INSTALL}/${PYTHON_ENV_DIRNAME}" CACHE INTERNAL "")

    get_filename_component(PYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE} NAME)
    if(WIN32)
        # When using the embedded python, python.exe is directly at the root,
        # but for a pure virtualenv it is in a 'Scripts' subfolder
        if(NOT EMBEDDED_PYTHON)
            set(PYTHON_EXECUTABLE_RELPATH "Scripts/${PYTHON_EXECUTABLE_RELPATH}")
        endif()
    else()
        # No embedded python on Linux for the moment (-> only a pure virtualenv)
        set(PYTHON_EXECUTABLE_RELPATH "bin/${PYTHON_EXECUTABLE_RELPATH}")
    endif()
    set(ISSOFAPYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE_RELPATH} CACHE INTERNAL "" FORCE)
    set(ISSOFAPYTHON_EXECUTABLE "${ISSOFAPYTHON_VIRTUALENV_DIR}/${PYTHON_EXECUTABLE_RELPATH}" CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "${ISSOFAPYTHON_PIP_INSTALL_OPTIONS}" CACHE STRING "" FORCE)

    # Put the local python in the build tree (only if not existing yet to keep short
    # configuration times when possible)
    if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE} OR ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED)
        if(EMBEDDED_PYTHON)
            # When fully embedding Python, no need to create a virtual env: instead we can
            # copy the entire Python which results in an improved virtual env which does not
            # rely on another Python installation
            message(STATUS "Copying the embedded python from ${EMBEDDED_PYTHON_DIR} to ${ISSOFAPYTHON_VIRTUALENV_DIR}")
            file(GLOB EMBEDDED_PYTHON_FILES "${EMBEDDED_PYTHON_DIR}/*")
            file(COPY ${EMBEDDED_PYTHON_FILES} DESTINATION ${ISSOFAPYTHON_VIRTUALENV_DIR}
                 PATTERN "*.pyc" EXCLUDE
                 PATTERN "include" EXCLUDE
                 PATTERN "libs" EXCLUDE
                 PATTERN "Scripts" EXCLUDE)
            # Also copy the Python DLL(s) next to the project executables in the build tree
            foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
                foreach(DLL ${EMBEDDED_PYTHON_DLLS})
                    file(COPY ${DLL} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}")
                endforeach()
            endforeach()
        else()
            message(STATUS "Creating python virtualenv in ${ISSOFAPYTHON_VIRTUALENV_DIR}")
            # Install the 'virtualenv' package (will do nothing if existing)
            execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install virtualenv)
            # Create the virtual env
            execute_process(COMMAND ${PYTHON_EXECUTABLE} -m virtualenv ${ISSOFAPYTHON_VIRTUALENV_OPTIONS} "${ISSOFAPYTHON_VIRTUALENV_DIR}")
            if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE})
                message(FATAL_ERROR "Failed to create python virtualenv executable ${ISSOFAPYTHON_EXECUTABLE}")
                return()
            endif()
        endif()
    endif()

    if(EMBEDDED_PYTHON)
        # Also install python in the install bin dir
        install(
            DIRECTORY "${EMBEDDED_PYTHON_DIR}/" # trailing slash to avoid having an extra folder level in the copy
            DESTINATION "${DEFAULT_BIN_INSTALL}/${PYTHON_ENV_DIRNAME}"
            PATTERN "*.pyc" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "libs" EXCLUDE
            PATTERN "Scripts" EXCLUDE
        )
        # We need the Python DLL next to the project executables
        install(FILES ${EMBEDDED_PYTHON_DLLS} DESTINATION ${DEFAULT_BIN_INSTALL})
    else()
        # No embedded python: for the install step we will re-create the virtual env
        # in the install bin dir.
        # We don't just copy it because a virtual env is not easily relocatable:
        # From the virtualenv's doc: the --relocatable option currently has a number of issues,
        # and is not guaranteed to work in all circumstances. It is possible that the option will
        # be deprecated in a future version of virtualenv.
        install(CODE "
            if(EXISTS \"${ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR}\")
                return()
            endif()
            message(STATUS \"Creating the python virtualenv in ${ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR}\")
            set(ENV{PYTHONDONTWRITEBYTECODE} 1)
            execute_process(
                COMMAND
                ${PYTHON_EXECUTABLE} -m virtualenv ${ISSOFAPYTHON_VIRTUALENV_OPTIONS} \"${ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR}\"
            )"
        )
    endif()

    # Use a sitecustomize.py file to easily adjust the python sys.path for our project
    set(SITECUSTOMIZE_FILE "${Base_SOURCE_DIR}/IS/ISSofaPython/build/sitecustomize.py.in")
    if (WIN32)
        set(SITECUSTOMIZE_DEST_DIR "${ISSOFAPYTHON_VIRTUALENV_DIR}/Lib")
        set(SITECUSTOMIZE_INSTALL_DEST_DIR "${DEFAULT_BIN_INSTALL}/${PYTHON_ENV_DIRNAME}/Lib")
    else()
        set(SITECUSTOMIZE_DEST_DIR "${ISSOFAPYTHON_VIRTUALENV_DIR}/lib/python2.7")
        set(SITECUSTOMIZE_INSTALL_DEST_DIR "${DEFAULT_BIN_INSTALL}/${PYTHON_ENV_DIRNAME}/lib/python2.7")
    endif()
    configure_file(${SITECUSTOMIZE_FILE} ${SITECUSTOMIZE_DEST_DIR}/sitecustomize.py)
    install(FILES ${SITECUSTOMIZE_DEST_DIR}/sitecustomize.py DESTINATION ${SITECUSTOMIZE_INSTALL_DEST_DIR})

    target_compile_definitions(ISSofaPythonPlugin PRIVATE "ISSOFAPYTHON_USE_VIRTUALENV=\"${ISSOFAPYTHON_USE_VIRTUALENV}\"")
    target_compile_definitions(ISSofaPythonPlugin PRIVATE "PYTHON_ENV_DIRNAME=\"${PYTHON_ENV_DIRNAME}\"")

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

    message(STATUS "Installing ${PROJECT_PYTHON_SETUPTOOLSFILE} with pip")
    execute_process(
        COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m pip install ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL} "./"
        WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
    )

    # Add a command for the install target. We don't install the packages
    # in editable mode (as this is more a convenience for dev/debug) that is
    # why we don't pass the ISSOFAPYTHON_PIP_INSTALL_OPTIONS here
    if (ISSOFAPYTHON_USE_VIRTUALENV)
        if(WIN32)
            set(PYTHON_SITE_PACKAGES_RELPATH "Lib/site-packages")
        else()
            set(PYTHON_SITE_PACKAGES_RELPATH "lib/python2.7/site-packages")
        endif()
        # Using the -t option of pip, explicitely specifying the site-packages dir
        # where to install, otherwise if a package is already installed on the system
        # it does not install it in our local site-packages
        # (I didn't find a suitable option for that)
        install(CODE "
            message(STATUS \"Installing ${PROJECT_PYTHON_SETUPTOOLSFILE} with pip\")
            set(ENV{PYTHONDONTWRITEBYTECODE} 1)
            execute_process(
                COMMAND
                ${ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR}/${ISSOFAPYTHON_EXECUTABLE_RELPATH} -m pip install \"./\"
                -t ${ISSOFAPYTHON_VIRTUALENV_INSTALL_DIR}/${PYTHON_SITE_PACKAGES_RELPATH}/
                WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
            )"
        )
    endif()
endfunction()
