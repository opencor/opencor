project(PythonPackagesPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION ${PYTHON_VERSION})

# Package repository and release tag

set(PACKAGE_REPOSITORY python)
set(RELEASE_TAG v${PYTHON_VERSION}_devel)

# The versions of the Python packages we bundle

set(NUMPY_VERSION 1.15.4)

set(MATPLOTLIB_VERSION 3.0.2)
set(MATPLOTLIB_OPENCOR 3.0.2.1)

set(SCIPY_VERSION 1.1.0)

set(IPYKERNEL_VERSION 5.1.0)
set(IPYKERNEL_PYTHONQT 5.1.0.1)

set(IPYTHON_VERSION 7.5.0)
set(IPYTHON_PYTHONQT 7.5.0.1)

set(QTCONSOLE_VERSION 4.4.4)
set(QTCONSOLE_PYTHONQT 4.4.4.1)

set(TRAITLETS_VERSION 4.3.2)
set(TRAITLETS_PYTHONQT 4.3.2.1)

set(NOTEBOOK_VERSION 5.7.8)
set(IPYWIDGETS_VERSION 7.4.2)
set(NBCONVERT_VERSION 5.5.0)
set(JUPYTER_CONSOLE_VERSION 6.0.0)

set(JUPYTERLAB_VERSION 0.35.6)

set(LXML_VERSION 4.2.5)

set(SPHINX_VERSION 1.8.2)


# Our GitHub organisation

set(GITHUB_OPENCOR https://github.com/opencor)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)
set(FULL_LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LOCAL_SITE_PACKAGES_DIR})

# Directories we package

set(PACKAGED_FILES
    etc
    include
    share
    ${LOCAL_SITE_PACKAGES_DIR}
    ${PYTHON_RELATIVE_SCRIPT_DIR}
)


# Specify the files that need to have their SHA-1 value checked
# Note: at least one file must have its SHA-1 value checked...

if(WIN32)
    set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/multiarray.cp${PYTHON_ABI_VERSION}-win_amd64.pyd)
elseif(APPLE)
    set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/multiarray.cpython-${PYTHON_ABI_VERSION}-darwin.so)
elseif(UNIX)
    set(SHA1_FILES ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/multiarray.cpython-${PYTHON_ABI_VERSION}-x86_64-linux-gnu.so)
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 652416df79cc3df5f658ce5960fb3bafe51fca6d
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES bd2befaf0efacb2d4ee4957ab58a4315290ec804)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 077f8f0f162fe54d1d4743b4ad43869d3683b8c7
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1b52d2a7320c27e0f4cd45bf98decde08d156969
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES de6bb8d119560aabd31d97cd1def0545ec3d88a6)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} c3dccf74c7d7ed11634839b17d0390a150e2ac1a
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 28a5b99a80b0d746764e3ac8c0f455787328a90f)
    endif()
else()
    # Install Python packages using pip

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    add_custom_target(${PACKAGE_BUILD})

    # Python packages installation requires Python

    set(BUILD_DEPENDENCIES PythonPlugin)

    # Ensure local site packages directory exists

    file(MAKE_DIRECTORY ${FULL_LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip

    file(TO_NATIVE_PATH ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX)

    set(PIP_INSTALL_COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
        ${PYTHON_EXECUTABLE} -s -m
        pip install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_PREFIX}
                    --upgrade --upgrade-strategy only-if-needed
                    --no-warn-script-location)

    # Make sure pip is up to date

    add_custom_target(upgradePip
        COMMAND ${PIP_INSTALL_COMMAND}
            pip
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} upgradePip)
    add_dependencies(upgradePip ${BUILD_DEPENDENCIES})

    # Install numpy, scipy, and matplotlib

    add_custom_target(sciencePackages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            numpy==${NUMPY_VERSION}
            scipy==${SCIPY_VERSION}
            matplotlib==${MATPLOTLIB_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} sciencePackages)
    add_dependencies(sciencePackages upgradePip)
    add_dependencies(sciencePackages ${BUILD_DEPENDENCIES})

    # Copy the numpy include directory to a place that OpenCOR can use

    add_custom_target(copyNumpyHeaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${FULL_LOCAL_SITE_PACKAGES_DIR}/numpy/core/include include
        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    )

    add_dependencies(${PACKAGE_BUILD} copyNumpyHeaders)
    add_dependencies(copyNumpyHeaders sciencePackages)
    add_dependencies(copyNumpyHeaders ${BUILD_DEPENDENCIES})

    # Install Jupyter, IPython and their dependencies
    # Note: order of installation matters as we want specific
    #       versions of each package

    add_custom_target(ipythonPackages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            traitlets==${TRAITLETS_VERSION}
            ipython==${IPYTHON_VERSION}
            ipykernel==${IPYKERNEL_VERSION}
            qtconsole==${QTCONSOLE_VERSION}
            notebook==${NOTEBOOK_VERSION}
            jupyter_console==${JUPYTER_CONSOLE_VERSION}
            nbconvert==${NBCONVERT_VERSION}
            ipywidgets==${IPYWIDGETS_VERSION}
            jupyterlab==${JUPYTERLAB_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} ipythonPackages)
    add_dependencies(ipythonPackages sciencePackages)
    add_dependencies(ipythonPackages ${BUILD_DEPENDENCIES})

    # Install other packages we use

    add_custom_target(otherPackages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            lxml==${LXML_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
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
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} installSphinx)
    add_dependencies(installSphinx otherPackages)
    add_dependencies(installSphinx ${BUILD_DEPENDENCIES})

    # We only upgrade to our versions of packages and patch matplotlib
    # *after* all 'standard' packages have been installed

    # Upgrade to our versions that have been patched to work with PythonQt

    set(ARCHIVE_PYTHONQT archive/PythonQt-v)

    add_custom_target(pythonQtUpdate
        COMMAND ${PIP_INSTALL_COMMAND} --no-deps --force-reinstall
            ${GITHUB_OPENCOR}/traitlets/${ARCHIVE_PYTHONQT}${TRAITLETS_PYTHONQT}.zip
            ${GITHUB_OPENCOR}/ipython/${ARCHIVE_PYTHONQT}${IPYTHON_PYTHONQT}.zip
            ${GITHUB_OPENCOR}/ipykernel/${ARCHIVE_PYTHONQT}${IPYKERNEL_PYTHONQT}.zip
            ${GITHUB_OPENCOR}/qtconsole/${ARCHIVE_PYTHONQT}${QTCONSOLE_PYTHONQT}.zip
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} pythonQtUpdate)
    add_dependencies(pythonQtUpdate installSphinx)
    add_dependencies(pythonQtUpdate ${BUILD_DEPENDENCIES})

    # Patch matplotlib's Qt backend so that it works with PythonQt

    add_custom_target(patchMatplotlib
        COMMAND ${PYTHON_EXECUTABLE} -s scripts/patch.py
            --strip=2
            --directory=${FULL_LOCAL_SITE_PACKAGES_DIR}/matplotlib
            --verbose
            src/matplotlib_${MATPLOTLIB_OPENCOR}.diff
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} patchMatplotlib)
    add_dependencies(patchMatplotlib pythonQtUpdate)
    add_dependencies(patchMatplotlib ${BUILD_DEPENDENCIES})

    # Package the newly installed Python packages

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
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
    ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
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
    EXTERNAL_DESTINATION_DIR
        ${PYTHON_ROOT_DIR}
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    DEPENDS_ON
        ${DEPENDS_ON}
)

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Update the Python path in newly copied scripts

add_custom_command(TARGET ${PROJECT_NAME}
                   COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_SCRIPT_DIR}/set_python_path.py
                                                --update-path ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_SCRIPT_DIR}
                                                ${PYTHON_ROOT_DIR} -s)
