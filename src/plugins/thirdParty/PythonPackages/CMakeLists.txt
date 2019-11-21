project(PythonPackagesPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION ${PYTHON_VERSION})

# Package repository and release tag

set(PACKAGE_REPOSITORY python)

# Git tags of Python package installers

set(PIP_GIT_TAG "19.2.3")
set(SETUPTOOLS_GIT_TAG "v41.2.0")

# Version of our different Python packages

set(IPYKERNEL_VERSION 5.1.2)
set(IPYTHON_VERSION 7.8.0)
set(IPYWIDGETS_VERSION 7.5.1)
set(JUPYTER_CONSOLE_VERSION 6.0.0)
set(JUPYTERLAB_VERSION 1.1.3)
set(LXML_VERSION 4.4.1)
set(MATPLOTLIB_VERSION 3.1.1)
set(NBCONVERT_VERSION 5.6.0)
set(NOTEBOOK_VERSION 6.0.1)
set(NUMPY_VERSION 1.17.2)
set(QIMAGE2NDARRAY_VERSION 1.8)
set(QTCONSOLE_VERSION 4.5.5)
set(SCIPY_VERSION 1.3.1)
set(SPHINX_VERSION 2.2.0)
set(TRAITLETS_VERSION 4.3.2)

# Patched version of some packages to support PythonQt

set(IPYKERNEL_PYTHONQT ${IPYKERNEL_VERSION}.0)
set(IPYTHON_PYTHONQT ${IPYTHON_VERSION}.0)
set(MATPLOTLIB_PYTHONQT ${MATPLOTLIB_VERSION}.1)
set(QTCONSOLE_PYTHONQT ${QTCONSOLE_VERSION}.0)
set(TRAITLETS_PYTHONQT ${TRAITLETS_VERSION}.1)

# Our GitHub organisation

set(GITHUB_OPENCOR https://github.com/opencor)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)
set(FULL_LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LOCAL_SITE_PACKAGES_DIR})

# Directories we package

set(PACKAGED_FILES
    include
    ${LOCAL_SITE_PACKAGES_DIR}
    ${PYTHON_RELATIVE_SCRIPT_DIR}
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

# Specify the files that need to have their SHA-1 value checked
# Note: at least one file must have its SHA-1 value checked...

if(WIN32)
    if(RELEASE_MODE)
        set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/_multiarray_umath.cp${PYTHON_ABI_VERSION}-win_amd64.pyd)
    else()
        set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/_multiarray_umath_d.cp${PYTHON_ABI_VERSION}-win_amd64.pyd)
    endif()
elseif(APPLE)
    set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/_multiarray_umath.cpython-${PYTHON_ABI_VERSION}-darwin.so)
elseif(UNIX)
    set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/_multiarray_umath.cpython-${PYTHON_ABI_VERSION}-x86_64-linux-gnu.so)
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 953af675f8f5ea21378d6a3dbc7ea68a4554e22f
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 360dfb74ccaca4d0152f5184c9adfeb4a65fd5d3)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} c406f6601bca4d86132d15cb62fb04e4b641255e
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 541c554a3e665c411a79517dac3afc45116c02ed)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} bccff03442bcab783691e8ce2f2da160b3084c67
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 4443d8cb01dcc0edc148f9d095201b13fd09f923)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} ecbb9933c748558c9dd41d2fe837489205cac3dd
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 80632ca1842b5a8668616766ef3bb2d01ed84e63)
    endif()
else()
    # Install Python packages using pip

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    add_custom_target(${PACKAGE_BUILD})

    # Python packages installation requires Python

    set(BUILD_DEPENDENCIES PythonPlugin)

    # Clean our site packages directory before installing any packages

    file(REMOVE_RECURSE ${FULL_LOCAL_SITE_PACKAGES_DIR})
    file(MAKE_DIRECTORY ${FULL_LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip when upgrading pip

    file(TO_NATIVE_PATH ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX)

    set(PIP_INSTALL_COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                                ${PYTHON_EXECUTABLE} -s -m pip install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX}
                                                                                       --upgrade
                                                                                       --upgrade-strategy only-if-needed
                                                                                       --no-warn-script-location)

    # Install the Python package manager and its dependencies
    # Note: on Windows, we use our version of pip that is modified to create
    #       *-script.py files when installing packages with console scripts, so
    #       that they can then be updated to reference OpenCOR's Python
    #       executable...

    set(PIP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/pip)

    if(WIN32)
        # Install the Python setuptools package

        set(SETUPTOOLS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/setuptools)

        ExternalProject_Add(installSetuptools
            DOWNLOAD_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            SOURCE_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            BINARY_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            INSTALL_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            GIT_REPOSITORY
                https://github.com/pypa/setuptools.git
            GIT_TAG
                ${SETUPTOOLS_GIT_TAG}
            CONFIGURE_COMMAND
                ${PYTHON_EXECUTABLE} bootstrap.py
            BUILD_COMMAND
                ${PYTHON_EXECUTABLE} setup.py build
            INSTALL_COMMAND
                ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                    ${PYTHON_EXECUTABLE} setup.py install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX}
        )

        # Install our Python package installer

        ExternalProject_Add(installPip
            DOWNLOAD_DIR
                ${PIP_SOURCE_DIR}
            SOURCE_DIR
                ${PIP_SOURCE_DIR}
            BINARY_DIR
                ${PIP_SOURCE_DIR}
            INSTALL_DIR
                ${PIP_SOURCE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/pip.git
            GIT_TAG
                opencor_v${PIP_GIT_TAG}
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                    ${PYTHON_EXECUTABLE} setup.py build
            INSTALL_COMMAND
                ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                    ${PYTHON_EXECUTABLE} setup.py install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX}
            DEPENDS
                installSetuptools
        )
    else()
        # Install the Python package installer

        ExternalProject_Add(installPip
            DOWNLOAD_DIR
                ${PIP_SOURCE_DIR}
            SOURCE_DIR
                ${PIP_SOURCE_DIR}
            BINARY_DIR
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
                ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
                                    ${PYTHON_EXECUTABLE} -s ${PIP_SOURCE_DIR}/get-pip.py --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX}
                                                                                         --no-wheel
        )

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
            DOWNLOAD_DIR
                ${NUMPY_SOURCE_DIR}
            SOURCE_DIR
                ${NUMPY_SOURCE_DIR}
            BINARY_DIR
                ${NUMPY_SOURCE_DIR}
            INSTALL_DIR
                ${NUMPY_SOURCE_DIR}
            URL
                https://github.com/numpy/numpy/releases/download/v${NUMPY_VERSION}/numpy-${NUMPY_VERSION}.zip
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
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
        # Install NumPy, SciPy and Matplotlib

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
                                           jupyter_console==${JUPYTER_CONSOLE_VERSION}
                                           jupyterlab==${JUPYTERLAB_VERSION}
                                           nbconvert==${NBCONVERT_VERSION}
                                           notebook==${NOTEBOOK_VERSION}
                                           qtconsole==${QTCONSOLE_VERSION}
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

        set(ARCHIVE_PYTHONQT archive/PythonQt-v)

        add_custom_target(pythonQtUpdate
            COMMAND ${PIP_INSTALL_COMMAND} --no-deps
                                           --force-reinstall
                                           ${GITHUB_OPENCOR}/ipykernel/${ARCHIVE_PYTHONQT}${IPYKERNEL_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/ipython/${ARCHIVE_PYTHONQT}${IPYTHON_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/qtconsole/${ARCHIVE_PYTHONQT}${QTCONSOLE_PYTHONQT}.zip
                                           ${GITHUB_OPENCOR}/traitlets/${ARCHIVE_PYTHONQT}${TRAITLETS_PYTHONQT}.zip
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

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES ${PACKAGED_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other packages to use our scripts

if(WIN32)
    set(SPHINX_EXECUTABLE sphinx-build.exe)
else()
    set(SPHINX_EXECUTABLE sphinx-build)
endif()

set(SPHINX_EXECUTABLE
    ${CMAKE_COMMAND} -E env PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}
                        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_SCRIPT_DIR}/${SPHINX_EXECUTABLE}
    PARENT_SCOPE
)

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

# Update the Python path in our newly copied scripts

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_SCRIPT_DIR}/set_python_path.py --scripts-dir ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_SCRIPT_DIR}
                                                                                                      ${PYTHON_ROOT_DIR}
                                                -s)

# Update our list of Python specific dependencies

set(PYTHON_DEPENDENCIES ${PYTHON_DEPENDENCIES} ${PROJECT_NAME} PARENT_SCOPE)
