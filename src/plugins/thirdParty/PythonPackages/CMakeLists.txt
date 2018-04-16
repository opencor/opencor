project(PythonPackagesPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION ${PYTHON_VERSION})

# The repository for our package

set(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

set(NUMPY_VERSION 1.14.2)
set(MATPLOTLIB_VERSION 2.2.2)
set(SCIPY_VERSION 1.0.1)

set(IPYKERNEL_VERSION 4.8.2)
set(IPYTHON_VERSION 6.3.1)
set(QTCONSOLE_VERSION 4.3.1)
set(TRAITLETS_VERSION 4.3.2)

set(LXML_VERSION 4.2.1)

# Our GitHub archive

set(GITHUB_OPENCOR https://github.com/opencor)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/site-packages)

# Package the include and site-packages directory

set(PACKAGED_FILES
    include
    site-packages
)

# Some files need to have their SHA-1 value checked

if(WIN32)
    set(SHA1_FILES site-packages/numpy/core/multiarray.cp${PYTHON_ABI_VERSION}-win_amd64.pyd)
elseif(APPLE)
    set(SHA1_FILES site-packages/numpy/core/multiarray.cpython-${PYTHON_ABI_VERSION}-darwin.so)
elseif(UNIX)
    set(SHA1_FILES site-packages/numpy/core/multiarray.cpython-${PYTHON_ABI_VERSION}-x86_64-linux-gnu.so)
endif()

# Use the pre-built version of our package unless instructed otherwise
# Rebuilding Python means we should also re-install its packages

if(USE_PREBUILT_PYTHON_PACKAGE AND USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 5dd535bc479f747597ebab5739c0ce967416bef6
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES bd2befaf0efacb2d4ee4957ab58a4315290ec804)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 97eff767ed79fe7ad9087cf161a218381a573804
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 13f1ca249d663cd76ea70873b659fa2a0c90e6bc)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 5b26aac0190f0cb1e5b58ee3c34639e339dae135
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 87041b5f0c84bca528d15de443cb9f0e35fa53e0)
    endif()
else()
    # Install Python packages using pip

    set(PACKAGE_BUILD PythonPackagesBuild)

    add_custom_target(${PACKAGE_BUILD})

    # Python package installation requires Python

    set(BUILD_DEPENDENCIES PythonPlugin)

    # Ensure local site packages directory exists

    file(MAKE_DIRECTORY ${LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip

    set(PIP_INSTALL_COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${LOCAL_SITE_PACKAGES_DIR}"
        ${PYTHON_EXECUTABLE} -s -m
        pip install -t ${LOCAL_SITE_PACKAGES_DIR} --upgrade --upgrade-strategy only-if-needed)

    # Install numpy, scipy, and matplotlib

    add_custom_target(science_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            numpy==${NUMPY_VERSION}
            scipy==${SCIPY_VERSION}
            matplotlib==${MATPLOTLIB_VERSION}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} science_packages)
    add_dependencies(science_packages ${BUILD_DEPENDENCIES})

    # Copy the numpy include directory to a place that OpenCOR can use

    add_custom_target(copy_numpy_headers
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/include include
        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    )

    add_dependencies(${PACKAGE_BUILD} copy_numpy_headers)
    add_dependencies(copy_numpy_headers science_packages)

    # Patch matplotlib's Qt backend so that it works with PythonQt

    add_custom_target(patch_matplotlib
        COMMAND ${PYTHON_EXECUTABLE} -s scripts/patch.py
            --strip=2
            --directory=${LOCAL_SITE_PACKAGES_DIR}/matplotlib
            src/matplotlib_${MATPLOTLIB_VERSION}.diff
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} patch_matplotlib)
    add_dependencies(patch_matplotlib science_packages)
    add_dependencies(patch_matplotlib ${BUILD_DEPENDENCIES})

    # Install IPython and its dependencies

    add_custom_target(ipython_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            traitlets==${TRAITLETS_VERSION}
            ipython==${IPYTHON_VERSION}
            ipykernel==${IPYKERNEL_VERSION}
            qtconsole==${QTCONSOLE_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} ipython_packages)
    add_dependencies(ipython_packages patch_matplotlib)
    add_dependencies(ipython_packages ${BUILD_DEPENDENCIES})

    # Install other packages we use

    add_custom_target(other_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            lxml==${LXML_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} other_packages)
    add_dependencies(other_packages ipython_packages)
    add_dependencies(other_packages ${BUILD_DEPENDENCIES})

    # Upgrade to our versions of packages which have been patched to work with PythonQt

    set(ARCHIVE_PYTHONQT archive/PythonQt-v)

    add_custom_target(pythonqt_update
        COMMAND ${PIP_INSTALL_COMMAND} --no-deps --force-reinstall
            ${GITHUB_OPENCOR}/traitlets/${ARCHIVE_PYTHONQT}${TRAITLETS_VERSION}.zip
            ${GITHUB_OPENCOR}/ipython/${ARCHIVE_PYTHONQT}${IPYTHON_VERSION}.zip
            ${GITHUB_OPENCOR}/ipykernel/${ARCHIVE_PYTHONQT}${IPYKERNEL_VERSION}.zip
            ${GITHUB_OPENCOR}/qtconsole/${ARCHIVE_PYTHONQT}${QTCONSOLE_VERSION}.zip
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} pythonqt_update)
    add_dependencies(pythonqt_update ipython_packages)
    add_dependencies(pythonqt_update ${BUILD_DEPENDENCIES})

    # Package the newly installed Python packages

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
        PACKAGED_FILES ${PACKAGED_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
        )

    set(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Add the plugin

add_plugin(PythonPackages
    SOURCES
        ../../plugininfo.cpp

        src/pythonpackagesplugin.cpp
    HEADERS_MOC
        src/pythonpackagesplugin.h
    PLUGINS
        Python
    EXTERNAL_DESTINATION_DIR
        ${PYTHON_SITE_PACKAGES_DIR}
    EXTERNAL_SOURCE_DIR
        ${LOCAL_SITE_PACKAGES_DIR}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
