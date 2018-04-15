PROJECT(PythonPackagesPlugin)

# Name and version of our package

SET(PACKAGE_NAME PythonPackages)
SET(PACKAGE_VERSION ${PYTHON_VERSION})

# The repository for our package

SET(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

SET(NUMPY_VERSION 1.14.2)
SET(MATPLOTLIB_VERSION 2.2.2)
SET(SCIPY_VERSION 1.0.1)

SET(IPYKERNEL_VERSION 4.8.2)
SET(IPYTHON_VERSION 6.3.1)
SET(QTCONSOLE_VERSION 4.3.1)
SET(TRAITLETS_VERSION 4.3.2)

SET(LXML_VERSION 4.2.1)

# Our GitHub archive

SET(GITHUB_OPENCOR https://github.com/opencor)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

SET(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/site-packages)

# Package the include and site-packages directory

SET(PACKAGED_FILES
    include
    site-packages
)

# Some files need to have their SHA-1 value checked
## "35m" needs to come from say ${PYTHON_ABI_VERSION}

IF(WIN32)
    SET(SHA1_FILES site-packages/numpy/core/multiarray.cp35-win_amd64.pyd)
ELSEIF(APPLE)
    SET(SHA1_FILES site-packages/numpy/core/multiarray.cpython-35m-darwin.so)
ELSEIF(UNIX)
    SET(SHA1_FILES site-packages/numpy/core/multiarray.cpython-35m-x86_64-linux-gnu.so)
ENDIF()

# Use the pre-built version of our package unless instructed otherwise
# Rebuilding Python means we should also re-install its packages

IF(USE_PREBUILT_PYTHON_PACKAGE AND USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 47119dd449b75490a90c18d8cc71945af080247f
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 97949f3f839adab391fc785d8f7459f569f63a4f)
        ENDIF()
    ELSEIF(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 10c77b619d7b98ca33250c379b5899ebddb3d9ce
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 13f1ca249d663cd76ea70873b659fa2a0c90e6bc)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 5b26aac0190f0cb1e5b58ee3c34639e339dae135
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 87041b5f0c84bca528d15de443cb9f0e35fa53e0)
    ENDIF()
ELSE()
    # Install Python packages using pip

    SET(PACKAGE_BUILD PythonPackagesBuild)

    ADD_CUSTOM_TARGET(${PACKAGE_BUILD})

    # Python package installation requires Python

    SET(BUILD_DEPENDENCIES PythonPlugin)

    # Ensure local site packages directory exists

    FILE(MAKE_DIRECTORY ${LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip

    SET(PIP_INSTALL_COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${LOCAL_SITE_PACKAGES_DIR}"
        ${PYTHON_EXECUTABLE} -s -m
        pip install -t ${LOCAL_SITE_PACKAGES_DIR} --upgrade --upgrade-strategy only-if-needed)

    # Install numpy, scipy, and matplotlib

    ADD_CUSTOM_TARGET(science_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            numpy==${NUMPY_VERSION}
            scipy==${SCIPY_VERSION}
            matplotlib==${MATPLOTLIB_VERSION}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} science_packages)
    ADD_DEPENDENCIES(science_packages ${BUILD_DEPENDENCIES})

    # Copy the numpy include directory to a place that OpenCOR can use

    ADD_CUSTOM_TARGET(copy_numpy_headers
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LOCAL_SITE_PACKAGES_DIR}/numpy/core/include include
        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} copy_numpy_headers)
    ADD_DEPENDENCIES(copy_numpy_headers science_packages)

    # Patch matplotlib's Qt backend so that it works with PythonQt

    ADD_CUSTOM_TARGET(patch_matplotlib
        COMMAND ${PYTHON_EXECUTABLE} -s scripts/patch.py
            --strip=2
            --directory=${LOCAL_SITE_PACKAGES_DIR}/matplotlib
            src/matplotlib_${MATPLOTLIB_VERSION}.diff
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} patch_matplotlib)
    ADD_DEPENDENCIES(patch_matplotlib science_packages)
    ADD_DEPENDENCIES(patch_matplotlib ${BUILD_DEPENDENCIES})

    # Install IPython and its dependencies

    ADD_CUSTOM_TARGET(ipython_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            traitlets==${TRAITLETS_VERSION}
            ipython==${IPYTHON_VERSION}
            ipykernel==${IPYKERNEL_VERSION}
            qtconsole==${QTCONSOLE_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} ipython_packages)
    ADD_DEPENDENCIES(ipython_packages patch_matplotlib)
    ADD_DEPENDENCIES(ipython_packages ${BUILD_DEPENDENCIES})

    # Install other packages we use

    ADD_CUSTOM_TARGET(other_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            lxml==${LXML_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} other_packages)
    ADD_DEPENDENCIES(other_packages ipython_packages)
    ADD_DEPENDENCIES(other_packages ${BUILD_DEPENDENCIES})

    # Upgrade to our versions of packages which have been patched to work with PythonQt

    SET(ARCHIVE_PYTHONQT archive/PythonQt-v)

    SET(DJB_BUILD /Users/dave/build)

    ADD_CUSTOM_TARGET(pythonqt_update
        COMMAND ${PIP_INSTALL_COMMAND} --no-deps
            ${GITHUB_OPENCOR}/traitlets/${ARCHIVE_PYTHONQT}${TRAITLETS_VERSION}.zip
            ${GITHUB_OPENCOR}/ipython/${ARCHIVE_PYTHONQT}${IPYTHON_VERSION}.zip
            ${GITHUB_OPENCOR}/ipykernel/${ARCHIVE_PYTHONQT}${IPYKERNEL_VERSION}.zip
            ${GITHUB_OPENCOR}/qtconsole/${ARCHIVE_PYTHONQT}${QTCONSOLE_VERSION}.zip
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} pythonqt_update)
    ADD_DEPENDENCIES(pythonqt_update ipython_packages)
    ADD_DEPENDENCIES(pythonqt_update ${BUILD_DEPENDENCIES})

    # Package the newly installed Python packages

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
        PACKAGED_FILES ${PACKAGED_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
        )

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Add the plugin

ADD_PLUGIN(PythonPackages
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
