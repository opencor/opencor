PROJECT(PythonPackagesPlugin)

# Name and version of our package

SET(PACKAGE_NAME PythonPackages)
SET(PACKAGE_VERSION ${PYTHON_VERSION})

# The repository for our package

SET(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

SET(NUMPY_VERSION 1.12.1)
SET(MATPLOTLIB_VERSION 1.5.3)
SET(SCIPY_VERSION 0.19.0)

SET(IPYKERNEL_VERSION 4.6.1)
SET(IPYTHON_VERSION 6.0.0)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

SET(WHEEL_REPOSITORY https://github.com/opencor/python/releases/download/wheels)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

SET(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/site-packages)

# Package the site-packages directory

SET(PACKAGED_FILES site-packages)

# XXXNo files need to have their SHA-1 value checked
## "35m" needs to come from say ${PYTHON_ABI_VERSION}

IF(APPLE)
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
            RETRIEVE_PACKAGE_FILE(PythonPackages ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 1c833ffa55c9cb0ff589193d67b5f46bc131398e
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 79175eea34add07fd68ae2b1a8a065a131145f3f
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 259b675eec7c9138893798095aea9ec468b87a37)
    ELSE()
        RETRIEVE_PACKAGE_FILE(PythonPackages ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 0c8b8b07d6af7cdc8e6746cb472d9be5f45f20f0
        )
    ENDIF()
ELSE()

    SET(PACKAGE_BUILD PythonPackagesBuild)

    ADD_CUSTOM_TARGET(${PACKAGE_BUILD})

    # Installing Python packages requires the Python library

    SET(BUILD_DEPENDENCIES PythonPlugin)

    # Ensure local site packages directory exists

    FILE(MAKE_DIRECTORY ${LOCAL_SITE_PACKAGES_DIR})

    # Use our installed pip

    SET(PIP_INSTALL_COMMAND ${PYTHON_EXECUTABLE} -s -m
        pip install -t ${LOCAL_SITE_PACKAGES_DIR} --upgrade --upgrade-strategy only-if-needed)

    # Install numpy, scipy, and matplotlib

    IF(WIN32)
        SET(WHEEL_SUFFIX "cp${NUMERIC_LIBRARY_VERSION}-cp${NUMERIC_LIBRARY_VERSION}${PYTHON_ABIFLAGS}-win_amd64.whl")

        # We use binary versions that are linked with the Intel Math Kernel Library

        SET(NUMPY_WHEEL "numpy-${NUMPY_VERSION}+mkl-${WHEEL_SUFFIX}")
        SET(SCIPY_WHEEL "scipy-${SCIPY_VERSION}-${WHEEL_SUFFIX}")
        SET(MATPLOTLIB_WHEEL "matplotlib-${MATPLOTLIB_VERSION}-${WHEEL_SUFFIX}")

        ADD_CUSTOM_TARGET(install_packages ${PIP_INSTALL_COMMAND}
            ${WHEEL_REPOSITORY}/${NUMPY_WHEEL}
            ${WHEEL_REPOSITORY}/${SCIPY_WHEEL}
            ${WHEEL_REPOSITORY}/${MATPLOTLIB_WHEEL}
        )
    ELSE()
        ADD_CUSTOM_TARGET(install_packages
            COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
                numpy==${NUMPY_VERSION}
                scipy==${SCIPY_VERSION}
                matplotlib==${MATPLOTLIB_VERSION}
                ipython==${IPYTHON_VERSION}
                ipykernel==${IPYKERNEL_VERSION}
            VERBATIM
        )
    ENDIF()

    ADD_DEPENDENCIES(${PACKAGE_BUILD} install_packages)
    ADD_DEPENDENCIES(install_packages ${BUILD_DEPENDENCIES})

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
    ADD_DEPENDENCIES(patch_matplotlib install_packages)
    ADD_DEPENDENCIES(patch_matplotlib ${BUILD_DEPENDENCIES})

    # Patch IPython so that it works with PythonQt

    ADD_CUSTOM_TARGET(patch_ipython
        COMMAND ${PYTHON_EXECUTABLE} -s scripts/patch.py
            --strip=1
            --directory=${LOCAL_SITE_PACKAGES_DIR}/IPython
            src/ipython_${IPYTHON_VERSION}.diff
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} patch_ipython)
    ADD_DEPENDENCIES(patch_ipython install_packages)
    ADD_DEPENDENCIES(patch_ipython ${BUILD_DEPENDENCIES})

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

# Allow other external projects to find our NumPy include directory

SET(NUMPY_INCLUDE_DIR ${PYTHON_SITE_PACKAGES_DIR}/numpy/core/include PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(PythonPackages
    SOURCES
        ../../plugininfo.cpp

        src/pythonpackagesplugin.cpp
    HEADERS_MOC
        src/pythonpackagesplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Python
    EXTERNAL_DESTINATION_DIR
        ${PYTHON_SITE_PACKAGES_DIR}
    EXTERNAL_SOURCE_DIR
        ${LOCAL_SITE_PACKAGES_DIR}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
