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

option(ISSOFAPYTHON_USE_LOCAL_ENV "Use a local python environment (fully standalone when available, else a virtualenv) for python packages instead of 'polluting' the system installation" ON)
set(ISSOFAPYTHON_VIRTUALENV_OPTIONS "--system-site-packages" CACHE STRING "If ISSOFAPYTHON_USE_LOCAL_ENV is ON: options given to the potential virtualenv creation command")
# Track a potential change of the virtualenv options, to re-execute the associated command in such case
if(NOT DEFINED ISSOFAPYTHON_VIRTUALENV_OPTIONS_old OR (ISSOFAPYTHON_VIRTUALENV_OPTIONS STREQUAL ISSOFAPYTHON_VIRTUALENV_OPTIONS_old))
    set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED FALSE)
else()
    set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_CHANGED TRUE)
endif()
set(ISSOFAPYTHON_VIRTUALENV_OPTIONS_old ${ISSOFAPYTHON_VIRTUALENV_OPTIONS} CACHE INTERNAL "Copy of ISSOFAPYTHON_VIRTUALENV_OPTIONS to track potential modifications")

cmake_dependent_option(ISSOFAPYTHON_INSTALL_USER "If ISSOFAPYTHON_USE_LOCAL_ENV is OFF: use --user option when installing python packages" ON
                       "NOT ISSOFAPYTHON_USE_LOCAL_ENV" OFF)
set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS "--editable" CACHE STRING "Options given to pip install commands")

set(ISSOFAPYTHON_ADDITIONAL_SYS_PATH_DIRS "" CACHE STRING "A semicolon separated list of project dirs to add to the python sys.path (relative to the root of the project)")

if(ISSOFAPYTHON_USE_LOCAL_ENV)
    # We also set a cached var with a generic name so that it can also be detected/used in other generic modules)
    set(PYTHON_USE_LOCAL_ENV TRUE CACHE BOOL "Whether or not a local Python environment is available and should be used" FORCE)

    # In the build tree, we put the content of the python env in a subfolder of ${CMAKE_BINARY_DIR}
    # to avoid having to copy and setup things it in each config (Debug, Release, ...)
    # in case of multi-config build.
    # Whereas in the install tree, we put the content of the python env directly in the same folder
    # as the other executables and DLLs.
    # This is to avoid as much as possible having to necessarily execute the binaries from the
    # directory containing the DLLs on Windows.
    if(EMBEDDED_PYTHON)
        set(ISSOFAPYTHON_LOCAL_ENV_DIR ${EMBEDDED_PYTHON_DIR} CACHE INTERNAL "")
        set(ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR "\${CMAKE_INSTALL_PREFIX}/${EMBEDDED_PYTHON_INSTALL_DIR}" CACHE INTERNAL "")
    else()
        # We will create a virtualenv
        if (WIN32)
            # On windows we put everything in the binaries folder
            set(ISSOFAPYTHON_LOCAL_ENV_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE INTERNAL "")
            set(ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR "\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE INTERNAL "")
        else()
            # On Linux things are split between bin and lib
            # In case we have simlinks, let's resolve them, as otherwise it can cause issues especially when installing a
            # virtual env with Python 3
            get_filename_component(CMAKE_BINARY_DIR_REALPATH ${CMAKE_BINARY_DIR} REALPATH)
            set(ISSOFAPYTHON_LOCAL_ENV_DIR ${CMAKE_BINARY_DIR_REALPATH} CACHE INTERNAL "")
            set(ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR "\${CMAKE_INSTALL_PREFIX}" CACHE INTERNAL "")
        endif()
    endif()

    get_filename_component(PYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE} NAME)
    if(WIN32)
        # When using the embedded python, python.exe is directly at the root,
        # but for a pure virtualenv it is in a 'Scripts' subfolder
        if(NOT EMBEDDED_PYTHON)
            set(PYTHON_EXECUTABLE_RELPATH "Scripts/${PYTHON_EXECUTABLE_RELPATH}")
        endif()
        set(PYTHON_SITE_PACKAGES_RELPATH "Lib/site-packages")
    else()
        # No embedded python on Linux for the moment (-> only a pure virtualenv)
        set(PYTHON_EXECUTABLE_RELPATH "bin/${PYTHON_EXECUTABLE_RELPATH}")
        set(PYTHON_SITE_PACKAGES_RELPATH "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages")
    endif()
    set(ISSOFAPYTHON_EXECUTABLE_RELPATH ${PYTHON_EXECUTABLE_RELPATH} CACHE INTERNAL "" FORCE)
    set(ISSOFAPYTHON_EXECUTABLE "${ISSOFAPYTHON_LOCAL_ENV_DIR}/${PYTHON_EXECUTABLE_RELPATH}" CACHE FILEPATH "Local python executable" FORCE)
    message(STATUS "ISSOFAPYTHON_EXECUTABLE=${ISSOFAPYTHON_EXECUTABLE}")
    set(ISSOFAPYTHON_SITE_PACKAGES_RELPATH "${PYTHON_SITE_PACKAGES_RELPATH}" CACHE INTERNAL "" FORCE)
    set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "${ISSOFAPYTHON_PIP_INSTALL_OPTIONS}" CACHE STRING "" FORCE)

    # Put the local python in the build tree (only if not existing yet to keep short
    # configuration times when possible).
    # When fully embedding Python, no need to create a virtual env: instead we copy
    # the entire Python which results in an improved virtual env which does not rely
    # on another Python installation (in this case the copy is done by ISExternals).
    if(NOT EMBEDDED_PYTHON)
        if (NOT WIN32)  # We create a virtual env only on Linux, but let's still do the test
            # Remove some files of the existing virtual env if any, that could cause issues when re-creating a virtual env
            set(_bin_dir ${ISSOFAPYTHON_LOCAL_ENV_DIR}/bin)
            file(GLOB _FILES_TO_RM ${_bin_dir}/python* ${_bin_dir}/activate* ${ISSOFAPYTHON_LOCAL_ENV_DIR}/bin/Activate* ${_bin_dir}/pip* ${_bin_dir}/easy_install* ${_bin_dir}/f2py* ${_bin_dir}/py* ${ISSOFAPYTHON_LOCAL_ENV_DIR}/bin/wheel*)
            if (_FILES_TO_RM)
                file(REMOVE_RECURSE ${_FILES_TO_RM})
            endif()
            file(GLOB _FILES_TO_RM ${ISSOFAPYTHON_LOCAL_ENV_DIR}/lib/python*)
            if (_FILES_TO_RM)
                file(REMOVE_RECURSE ${_FILES_TO_RM})
            endif()
        endif()

        message(STATUS "Creating a Python venv in ${ISSOFAPYTHON_LOCAL_ENV_DIR} using ${PYTHON_EXECUTABLE}")
        if (PYTHON_VERSION_MAJOR LESS 3)
            # Install the 'virtualenv' package (will do nothing if existing)
            execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install virtualenv)
            set(VIRTUALENV_PACKAGE "virtualenv")
        else()
            # Use 'venv' that directly comes with Python 3
            set(VIRTUALENV_PACKAGE "venv")
        endif()
        # Create the virtual env
        execute_process(
            COMMAND
            ${PYTHON_EXECUTABLE} -m ${VIRTUALENV_PACKAGE} --copies "${ISSOFAPYTHON_LOCAL_ENV_DIR}"
            RESULT_VARIABLE CMD_RC)
        if (CMD_RC)
            message(FATAL_ERROR "Failed to create the Python virtual env: error ${CMD_RC}")
        endif()
        if(NOT EXISTS ${ISSOFAPYTHON_EXECUTABLE})
            message(FATAL_ERROR "Failed to create the Python virtual env: ${ISSOFAPYTHON_EXECUTABLE} does not exist")
            return()
        else()
            message(STATUS "The virtual env was created")
        endif()
    endif()

    if(NOT EMBEDDED_PYTHON)
        # No full python distribution available: for the install step we will re-create the virtual env.
        # We don't just copy it because a virtual env is not easily relocatable:
        # From the virtualenv's doc: the --relocatable option currently has a number of issues,
        # and is not guaranteed to work in all circumstances. It is possible that the option will
        # be deprecated in a future version of virtualenv.
        install(CODE "
            if (EXISTS ${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}/${ISSOFAPYTHON_EXECUTABLE_RELPATH})
                message(STATUS \"The python virtualenv already exists in ${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}\")
            else()
                message(STATUS \"Creating the python virtualenv in ${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}\")
                set(ENV{PYTHONDONTWRITEBYTECODE} 1)
                execute_process(
                    COMMAND
                    ${PYTHON_EXECUTABLE} -m ${VIRTUALENV_PACKAGE} --copies \"${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}\"
                    RESULT_VARIABLE VIRTUALENV_CREATION_RC
                )
                if (VIRTUALENV_CREATION_RC)
                    message(FATAL_ERROR \"The python virtualenv creation failed\")
                endif()
            endif()
            "
        )
    endif()

    # Use a sitecustomize.py file to easily adjust the python sys.path for our project
    set(SITECUSTOMIZE_FILE "${ISSofaPython_SOURCE_DIR}/sitecustomize.py.in")
    if (WIN32)
        set(PY_LIB_DIR "${ISSOFAPYTHON_LOCAL_ENV_DIR}/Lib")
        set(INSTALL_PY_LIB_DIR "${CMAKE_INSTALL_BINDIR}/Lib")
    else()
        set(PY_LIB_DIR "${ISSOFAPYTHON_LOCAL_ENV_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
        set(INSTALL_PY_LIB_DIR "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}") # hardcoding 'lib' instead of ${DEFAULT_LIB_INSTALL} because if different, python won't work.
    endif()
    file(RELATIVE_PATH PROJECT_ROOT_REL_DIR ${PY_LIB_DIR} ${PROJECT_ROOT_DIR})
    configure_file(${SITECUSTOMIZE_FILE} ${PY_LIB_DIR}/sitecustomize.py)
    # Same thing for the install version
    file(RELATIVE_PATH PROJECT_ROOT_REL_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_PY_LIB_DIR}" ${PROJECT_ROOT_DIR})
    # We also put this version in the build tree, then it will be copied into the install tree at install time
    configure_file(${SITECUSTOMIZE_FILE} ${PY_LIB_DIR}/sitecustomize-install.py)
    install(FILES ${PY_LIB_DIR}/sitecustomize-install.py DESTINATION ${INSTALL_PY_LIB_DIR} RENAME sitecustomize.py)
    # Copy _is_prepare_env.pth and _is_prepare_env.py to ensure our sitecustomize.py will be imported
    configure_file(${ISSofaPython_SOURCE_DIR}/_is_prepare_env.pth ${PY_LIB_DIR}/site-packages COPYONLY)
    install(FILES ${PY_LIB_DIR}/site-packages/_is_prepare_env.pth DESTINATION ${INSTALL_PY_LIB_DIR}/site-packages)
    configure_file(${ISSofaPython_SOURCE_DIR}/_is_prepare_env.py ${PY_LIB_DIR}/site-packages COPYONLY)
    install(FILES ${PY_LIB_DIR}/site-packages/_is_prepare_env.py DESTINATION ${INSTALL_PY_LIB_DIR}/site-packages)

    target_compile_definitions(ISSofaPythonPlugin PRIVATE "ISSOFAPYTHON_USE_LOCAL_ENV=\"${ISSOFAPYTHON_USE_LOCAL_ENV}\"")

else()
    if(DEFINED PYTHON_USE_LOCAL_ENV)
        unset(PYTHON_USE_LOCAL_ENV CACHE)
    endif()

    if(ISSOFAPYTHON_INSTALL_USER)
        set(ISSOFAPYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} CACHE FILEPATH "Local python executable (within virtualenv if enabled)" FORCE)
        set(ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL "--user" ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS} CACHE STRING "" FORCE)
        if(NOT PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE)
            execute_process(COMMAND ${ISSOFAPYTHON_EXECUTABLE} -m site --user-site
                            OUTPUT_VARIABLE PYTHON_USER_SITE_DIR
                            OUTPUT_STRIP_TRAILING_WHITESPACE )
            set(PYTHON_PIP_DEVELOP_EASY_INSTALL_FILE "${PYTHON_USER_SITE_DIR}/easy-install.pth" CACHE "" INTERNAL)
        endif()
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
        COMMAND
        ${ISSOFAPYTHON_EXECUTABLE} -m pip install ${ISSOFAPYTHON_PIP_INSTALL_OPTIONS_ALL} "./"
        WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
        RESULT_VARIABLE PIP_INSTALL_RC
    )
    if(PIP_INSTALL_RC)
        message(FATAL_ERROR "pip install failed with code ${PIP_INSTALL_RC}")
    endif()

    # Add a command for the install target. We don't install the packages
    # in editable mode (as this is more a convenience for dev/debug) that is
    # why we don't pass the ISSOFAPYTHON_PIP_INSTALL_OPTIONS here
    if (ISSOFAPYTHON_USE_LOCAL_ENV)
        # - Using the -t option of pip, explicitely specifying the site-packages dir
        #   where to install, otherwise if a package is already installed on the system
        #   it does not install it in our local site-packages
        #   (I didn't find a suitable option for that)
        # - Two commands are executed:
        #   "pip install --upgrade --no-deps" and then "pip install"
        #   -> this is to force the upgrade of the main installed package, while only
        #   installing the dependencies if missing (but not forcing their upgrade).
        #   The --upgrade --upgrade-strategy=only-if-needed option combo was tried, but
        #   the dependencies were reinstalled, at least when we don't specify versions.
        #   (another solution could be to remove the site-packages/<main-package-name>
        #   folder before doing a single "pip install")
        set(PIP_INSTALL_CMD "${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}/${ISSOFAPYTHON_EXECUTABLE_RELPATH} -s -m pip install")
        set(PIP_INSTALL_DIR "${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}/${ISSOFAPYTHON_SITE_PACKAGES_RELPATH}")
        install(CODE "
            message(STATUS \"Installing ${PROJECT_PYTHON_SETUPTOOLSFILE} with pip [step 1: force upgrade, without dependencies]\")
            set(ENV{PYTHONDONTWRITEBYTECODE} 1) # seems to have more effect than python -B option
            execute_process(
                COMMAND
                ${PIP_INSTALL_CMD} \"./\" -t ${PIP_INSTALL_DIR} --upgrade --no-deps --disable-pip-version-check
                WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
                RESULT_VARIABLE PIP_INSTALL_RC
            )
            if(PIP_INSTALL_RC)
                message(FATAL_ERROR \"pip install failed with error '\${PIP_INSTALL_RC}'\")
            endif()
            message(STATUS \"Installing ${PROJECT_PYTHON_SETUPTOOLSFILE} with pip [step 2: install dependencies]\")
            execute_process(
                COMMAND
                ${PIP_INSTALL_CMD} \"./\" -t ${PIP_INSTALL_DIR} --disable-pip-version-check
                WORKING_DIRECTORY ${PROJECT_PYTHON_SETUPTOOLS_DIR}
                RESULT_VARIABLE PIP_INSTALL_RC
            )
            if(PIP_INSTALL_RC)
                message(FATAL_ERROR \"pip install failed with error '\${PIP_INSTALL_RC}'\")
            endif()
            "
        )
    endif()
endfunction()

# Install 3rd party python dependencies listed in a requirements file.
function(issofapython_install_requirements requirements_file_path)
    message(STATUS "Installing ${requirements_file_path} with pip")
    execute_process(
        COMMAND
        ${ISSOFAPYTHON_EXECUTABLE} -m pip install -r ${requirements_file_path}
        RESULT_VARIABLE PIP_INSTALL_RC
    )
    if(PIP_INSTALL_RC)
        message(FATAL_ERROR "pip install failed with code ${PIP_INSTALL_RC}")
    endif()

    # Add a command for the install target.
    # Using the python -s option to force installing packages in the local site-packages
    # dir of the project instead of potentially using the user site dir.
    if (ISSOFAPYTHON_USE_LOCAL_ENV)
        set(PIP_INSTALL_CMD "${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}/${ISSOFAPYTHON_EXECUTABLE_RELPATH} -s -m pip install")
        set(PIP_INSTALL_DIR "${ISSOFAPYTHON_LOCAL_ENV_INSTALL_DIR}/${ISSOFAPYTHON_SITE_PACKAGES_RELPATH}")
        install(CODE "
            message(STATUS \"Installing ${requirements_file_path} with pip\")
            set(ENV{PYTHONDONTWRITEBYTECODE} 1) # seems to have more effect than python -B option
            execute_process(
                COMMAND
                ${PIP_INSTALL_CMD} -r ${requirements_file_path} -t ${PIP_INSTALL_DIR} --disable-pip-version-check
                RESULT_VARIABLE PIP_INSTALL_RC
            )
            if(PIP_INSTALL_RC)
                message(FATAL_ERROR \"pip install failed with error '\${PIP_INSTALL_RC}'\")
            endif()
            "
        )
    endif()
endfunction()
