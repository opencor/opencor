project(PythonPackagesPlugin)

# Our package characteristics

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION 3.10.13)
set(PACKAGE_REPOSITORY python)
set(RELEASE_TAG v3.10.13)

get_full_local_external_package_dir()

# Version of our different Python packages

set(IPYKERNEL_VERSION 6.29.2)
set(IPYTHON_VERSION 8.21.0)
set(IPYWIDGETS_VERSION 8.1.1)
set(JUPYTER_CLIENT_VERSION 8.6.0)
set(JUPYTER_CONSOLE_VERSION 6.6.3)
set(JUPYTERLAB_VERSION 4.0.12)
set(LXML_VERSION 5.1.0)
set(MATPLOTLIB_VERSION 3.8.2)
set(NBCONVERT_VERSION 7.14.2)
set(NOTEBOOK_VERSION 7.0.7)
set(NUMPY_VERSION 1.26.3)
set(QIMAGE2NDARRAY_VERSION 1.10.0)
set(QTCONSOLE_VERSION 5.5.1)
set(QTPY_VERSION 2.4.1)
set(SCIPY_VERSION 1.12.0)
set(SPHINX_VERSION 7.2.6)
set(TRAITLETS_VERSION 5.14.1)

# Patched versions of some Python only packages neede to
# support PythonQt. These are tags in our copy of the source
# repositories

set(IPYKERNEL_PYTHONQT "v${IPYKERNEL_VERSION}-pythonqt.0")
set(IPYTHON_PYTHONQT "v${IPYTHON_VERSION}-pythonqt.0")
set(QTCONSOLE_PYTHONQT "v${QTCONSOLE_VERSION}-pythonqt.2")
set(QTPY_PYTHONQT "v${QTPY_VERSION}-pythonqt.1")
set(TRAITLETS_PYTHONQT "v${TRAITLETS_VERSION}-pythonqt.0")

# Matplotlib is patched using a `diff` file as installing to
# avoid having to create a full installable package for it,
# including binary libraries.

set(MATPLOTLIB_PYTHONQT ${MATPLOTLIB_VERSION}.1)

# We have our own version of pip for Windows

set(OPENCOR_PIP_VERSION v24.0-opencor.0)

# Our GitHub organisation

set(GITHUB_OPENCOR https://github.com/opencor)

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)
set(FULL_LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LOCAL_SITE_PACKAGES_DIR})

# Directories we package

set(PACKAGED_FILES
    include
    ${LOCAL_SITE_PACKAGES_DIR}
    ${PYTHON_RELATIVE_SCRIPTS_DIR}
)

# These directories only exist in release mode
# Note: indeed, in debug mode (on Windows), we only have what is needed to run
#       our tests and to do some basic debugging...

if(RELEASE_MODE OR NOT WIN32)
    list(APPEND PACKAGED_FILES
        etc
        share
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             e1fd63b7ffb61de6bef75471d838bffeb4c7773c)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             4b30113cdbfa654daeacb925fe334cb6ef22e39d)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         011520dc16765000df191da9daaaff09a164d3b3)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         ba09a8b219ab37c4d80fc518ce316c1cd8902707)
    endif()
else()
    # Install Python packages using pip

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    add_custom_target(${PACKAGE_BUILD})

    # Where to find libraries needed by Python at build time

    set(BUILT_LIBRARIES_DIR ${PYTHON_ROOT_DIR}/lib:${PROJECT_BUILD_DIR}/lib)

    # Python packages installation requires Python

    set(BUILD_DEPENDENCIES PythonPlugin)

    # Clean our site packages directory before installing any packages

    file(REMOVE_RECURSE ${FULL_LOCAL_SITE_PACKAGES_DIR})
    file(MAKE_DIRECTORY ${FULL_LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip when upgrading pip

    set(PIP_INSTALL_COMMAND ${CMAKE_COMMAND} -E env "PYTHONHOME=${PYTHON_ROOT_DIR}"
                                                env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                                ${PYTHON_EXECUTABLE} -s -m pip install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                                                                       --upgrade
                                                                                       --upgrade-strategy only-if-needed
                                                                                       --no-warn-script-location)

    # Install the Python package manager and its dependencies
    # Note: on Windows, we use our version of pip that is modified to create
    #       *-script.py files when installing packages with console scripts, so
    #       that they can then be updated to reference OpenCOR's Python
    #       executable...

    set(PIP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/pip)

    # Install the Python package installer

    ExternalProject_Add(installPip
        DOWNLOAD_DIR
            ${PIP_SOURCE_DIR}
        INSTALL_DIR
            ${PIP_SOURCE_DIR}
        URL
            https://bootstrap.pypa.io/get-pip.py
        DOWNLOAD_NO_EXTRACT
            TRUE
        CONFIGURE_COMMAND
            ""
        BUILD_COMMAND
            ""
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E env "PYTHONHOME=${PYTHON_ROOT_DIR}"
                                env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                ${PYTHON_EXECUTABLE} -s ${PIP_SOURCE_DIR}/get-pip.py --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                                                                     --no-wheel
        STEP_TARGETS install
        LOG_DIR
            "${CMAKE_SOURCE_DIR}/log"
        LOG_INSTALL TRUE
    )

    if(WIN32)
        # Now use pip to install our copy of itself

        set(OPENCOR_PIP_ARCHIVE
            https://github.com/opencor/pip/archive/refs/tags/${OPENCOR_PIP_VERSION}.tar.gz)

        ExternalProject_Add_Step(installPip replacePip
                                 COMMAND ${PIP_INSTALL_COMMAND}
                                           --no-deps --force-reinstall
                                           ${OPENCOR_PIP_ARCHIVE}
                                 WORKING_DIRECTORY ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
                                 DEPENDEES install)
    else()
        # Make sure that pip is up to date

        ExternalProject_Add_Step(installPip updatePip
                                 COMMAND ${PIP_INSTALL_COMMAND} pip
                                 WORKING_DIRECTORY ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
                                 DEPENDEES install)
    endif()

    add_dependencies(${PACKAGE_BUILD} installPip)
    add_dependencies(installPip ${BUILD_DEPENDENCIES})

    # On Windows, we only install a minimal set of packages when building in
    # debug mode, namely a minimal version of NumPy (used to interface with our
    # DataStore arrays) and Sphinx (used to build our documentation)

    if (WIN32 AND NOT RELEASE_MODE)
        # Download, build and install a debug version of NumPy
        # Note: this is a minimal build since we don't provide linear algebra
        #       libraries nor a Fortran compiler...

        set(NUMPY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/numpy)

        ExternalProject_Add(installNumpy
            INSTALL_DIR
                ${NUMPY_SOURCE_DIR}
            URL
                https://github.com/numpy/numpy/releases/download/v${NUMPY_VERSION}/numpy-${NUMPY_VERSION}.zip
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${CMAKE_COMMAND} -E env "PYTHONHOME=${PYTHON_ROOT_DIR}"
                                    env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                    ${PYTHON_EXECUTABLE} setup.py build --debug
            INSTALL_COMMAND
                ${PIP_INSTALL_COMMAND} --global-option=build
                                       --global-option=--debug
                                       .
        )

        add_dependencies(${PACKAGE_BUILD} installNumpy)
        add_dependencies(installNumpy installPip)
        add_dependencies(installNumpy ${BUILD_DEPENDENCIES})

        # Copy the NumPy include directory to a place that OpenCOR can use

        add_custom_target(copyNumpyHeaders
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${FULL_LOCAL_SITE_PACKAGES_DIR}/numpy/core/include
                                                       include
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        )

        add_dependencies(${PACKAGE_BUILD} copyNumpyHeaders)
        add_dependencies(copyNumpyHeaders installNumpy)
        add_dependencies(copyNumpyHeaders ${BUILD_DEPENDENCIES})

        # Install Sphinx

        add_custom_target(installSphinx
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                                           sphinx==${SPHINX_VERSION}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} installSphinx)
        add_dependencies(installSphinx installNumpy)
        add_dependencies(installSphinx ${BUILD_DEPENDENCIES})
    else()
        # Install Matplotlib, NumPy, and SciPy

        add_custom_target(sciencePackages
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                                           matplotlib==${MATPLOTLIB_VERSION}
                                           numpy==${NUMPY_VERSION}
                                           qimage2ndarray==${QIMAGE2NDARRAY_VERSION}
                                           scipy==${SCIPY_VERSION}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} sciencePackages)
        add_dependencies(sciencePackages installPip)
        add_dependencies(sciencePackages ${BUILD_DEPENDENCIES})

        # Copy the NumPy include directory to a place that OpenCOR can use

        add_custom_target(copyNumpyHeaders
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${FULL_LOCAL_SITE_PACKAGES_DIR}/numpy/core/include
                                                       include
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        )

        add_dependencies(${PACKAGE_BUILD} copyNumpyHeaders)
        add_dependencies(copyNumpyHeaders sciencePackages)
        add_dependencies(copyNumpyHeaders ${BUILD_DEPENDENCIES})

        # Install Jupyter, IPython and their dependencies

        add_custom_target(ipythonPackages
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                                           ipykernel==${IPYKERNEL_VERSION}
                                           ipython==${IPYTHON_VERSION}
                                           ipywidgets==${IPYWIDGETS_VERSION}
                                           jupyter_client==${JUPYTER_CLIENT_VERSION}
                                           jupyter_console==${JUPYTER_CONSOLE_VERSION}
                                           jupyterlab==${JUPYTERLAB_VERSION}
                                           nbconvert==${NBCONVERT_VERSION}
                                           notebook==${NOTEBOOK_VERSION}
                                           qtconsole==${QTCONSOLE_VERSION}
                                           qtpy==${QTPY_VERSION}
                                           traitlets==${TRAITLETS_VERSION}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} ipythonPackages)
        add_dependencies(ipythonPackages sciencePackages)
        add_dependencies(ipythonPackages ${BUILD_DEPENDENCIES})

        # Install other packages that we use

        add_custom_target(otherPackages
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                                           lxml==${LXML_VERSION}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} otherPackages)
        add_dependencies(otherPackages ipythonPackages)
        add_dependencies(otherPackages ${BUILD_DEPENDENCIES})

        # Install Sphinx

        add_custom_target(installSphinx
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                                           sphinx==${SPHINX_VERSION}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} installSphinx)
        add_dependencies(installSphinx otherPackages)
        add_dependencies(installSphinx ${BUILD_DEPENDENCIES})

        # Upgrade to our versions that have been patched to work with PythonQt
        # Note: we only upgrade to our version of packages and patch Matplotlib
        #       *after* all standard packages have been installed...

        add_custom_target(pythonQtUpdate
            COMMAND ${PIP_INSTALL_COMMAND} --no-deps
                                           --force-reinstall
                                           ${GITHUB_OPENCOR}/ipykernel/archive/${IPYKERNEL_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/ipython/archive/${IPYTHON_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/qtconsole/archive/${QTCONSOLE_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/qtpy/archive/${QTPY_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/traitlets/archive/${TRAITLETS_PYTHONQT}.zip
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} pythonQtUpdate)
        add_dependencies(pythonQtUpdate installSphinx)
        add_dependencies(pythonQtUpdate ${BUILD_DEPENDENCIES})

        # Patch Matplotlib's Qt backend so that it works with PythonQt

        add_custom_target(patchMatplotlib
            COMMAND ${PYTHON_EXECUTABLE} -s scripts/patch.py --strip=2
                                                             --directory=${FULL_LOCAL_SITE_PACKAGES_DIR}/matplotlib
                                                             --verbose
                                                             src/matplotlib_${MATPLOTLIB_PYTHONQT}.diff
            WORKING_DIRECTORY
                ${PROJECT_SOURCE_DIR}
            VERBATIM
        )

        add_dependencies(${PACKAGE_BUILD} patchMatplotlib)
        add_dependencies(patchMatplotlib pythonQtUpdate)
        add_dependencies(patchMatplotlib ${BUILD_DEPENDENCIES})
    endif()

    # Package the newly installed Python packages

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   ${PACKAGED_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Add the plugin

add_plugin(PythonPackages
    SOURCES
        ../../plugininfo.cpp

        src/pythonpackagesplugin.cpp
    PLUGINS
        Python
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    EXTERNAL_DESTINATION_DIR
        ${PYTHON_ROOT_DIR}
    DEPENDS_ON
        ${DEPENDS_ON}
)

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Update the Python path in our Python scripts

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
                       POST_BUILD
                       COMMAND ${PYTHON_EXECUTABLE}
                               ${PYTHON_SET_PYTHON_PATH_SCRIPT} --scripts-dir ${PYTHON_SCRIPTS_DIR}
                                                                ${PYTHON_ROOT_DIR}
                               -s)
else()
    add_custom_command(TARGET ${PROJECT_NAME}
                       POST_BUILD
                       COMMAND ${PYTHON_EXECUTABLE}
                               ${PYTHON_SET_PYTHON_PATH_SCRIPT} ${PYTHON_SCRIPTS_DIR})
endif()

# Update our list of Python specific dependencies

set(PYTHON_DEPENDENCIES ${PYTHON_DEPENDENCIES} ${PROJECT_NAME} PARENT_SCOPE)
