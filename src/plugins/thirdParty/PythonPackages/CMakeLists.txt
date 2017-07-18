PROJECT(PythonPackagesPlugin)

# Name and version of our package

SET(PACKAGE_NAME PythonPackages)
SET(PACKAGE_VERSION ${PYTHON_VERSION})

# The repository for our package

SET(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

SET(NUMPY_VERSION 1.13.0)
SET(MATPLOTLIB_VERSION 2.0.2)
SET(SCIPY_VERSION 0.19.0)

SET(IPYKERNEL_VERSION 4.6.1)
SET(IPYTHON_VERSION 6.1.0)
SET(QTCONSOLE_VERSION 4.3.0)
SET(TRAITLETS_VERSION 4.3.2)

# Our GitHub archive

SET(GITHUB_OPENCOR https://github.com/opencor)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

SET(WHEEL_REPOSITORY ${GITHUB_OPENCOR}/python/releases/download/wheels)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

SET(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/site-packages)

# Package the include and site-packages directory

SET(PACKAGED_FILES
    include
    site-packages
)

# XXXNo files need to have their SHA-1 value checked
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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} cc2e641040bb118b2707c82e1cd1bcb4e9dc1834
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 97949f3f839adab391fc785d8f7459f569f63a4f)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 3581400b929ebe2c43668bc95b5be515045729e7
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 404333b7075ec857401b22c5de977ad5869dd6fc)
    ELSE()
        RETRIEVE_PACKAGE_FILE(PythonPackages ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 0c8b8b07d6af7cdc8e6746cb472d9be5f45f20f0
        )
    ENDIF()
ELSE()

    SET(PACKAGE_BUILD PythonPackagesBuild)

    ADD_CUSTOM_TARGET(${PACKAGE_BUILD})

    # Installing Python packages requires the Python library

    SET(BUILD_DEPENDENCIES PythonPlugin_EXTERNAL_SOURCES)

    # Ensure local site packages directory exists

    FILE(MAKE_DIRECTORY ${LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip

    SET(PIP_INSTALL_COMMAND
        ${CMAKE_COMMAND} -E env "PYTHONPATH=${LOCAL_SITE_PACKAGES_DIR}"
        ${PYTHON_EXECUTABLE} -s -m
        pip install -t ${LOCAL_SITE_PACKAGES_DIR} --upgrade --upgrade-strategy only-if-needed)

    # Install numpy, scipy, and matplotlib

    IF(WIN32)
        STRING(REPLACE "." "" VERSION_ABIFLAGS ${PYTHON_FULL_VERSION})
        STRING(SUBSTRING ${VERSION_ABIFLAGS} 0 2 NUMERIC_VERSION)
        SET(WHEEL_SUFFIX "cp${NUMERIC_VERSION}-cp${VERSION_ABIFLAGS}-win_amd64.whl")

        # We use binary versions that are linked with the Intel Math Kernel Library

        SET(NUMPY_WHEEL "numpy-${NUMPY_VERSION}.mkl-${WHEEL_SUFFIX}")
        SET(SCIPY_WHEEL "scipy-${SCIPY_VERSION}-${WHEEL_SUFFIX}")
        SET(MATPLOTLIB_WHEEL "matplotlib-${MATPLOTLIB_VERSION}-${WHEEL_SUFFIX}")

        ADD_CUSTOM_TARGET(science_packages
            COMMAND ${PIP_INSTALL_COMMAND}
                ${WHEEL_REPOSITORY}/${NUMPY_WHEEL}
                ${WHEEL_REPOSITORY}/${SCIPY_WHEEL}
                ${WHEEL_REPOSITORY}/${MATPLOTLIB_WHEEL}
            WORKING_DIRECTORY
                ${FULL_DEST_EXTERNAL_BINARIES_DIR}
            VERBATIM
        )
    ELSE()
        ADD_CUSTOM_TARGET(science_packages
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                numpy==${NUMPY_VERSION}
                scipy==${SCIPY_VERSION}
                matplotlib==${MATPLOTLIB_VERSION}
            VERBATIM
        )
    ENDIF()

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

    # Upgrade to our versions of packages which have been patched to work with PythonQt

    SET(ARCHIVE_PYTHONQT archive/PythonQt-v)

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
