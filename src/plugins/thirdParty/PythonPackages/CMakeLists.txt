PROJECT(PythonPackagesPlugin)

# Name and version of our package

SET(PACKAGE_NAME PythonPackages)
SET(PACKAGE_VERSION ${PYTHON_VERSION})
SET(RELEASE_TAG v${PYTHON_VERSION}_base)

# The repository for our package

SET(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

SET(SPHINX_VERSION 1.7.2)

# Our GitHub archive

SET(GITHUB_OPENCOR https://github.com/opencor)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

SET(WHEEL_REPOSITORY ${GITHUB_OPENCOR}/python/releases/download/wheels)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

SET(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)

# Package the `bin`, `include` and `lib` directories

SET(PACKAGED_FILES
    bin
    ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages
)

# At least one file must have its SHA-1 value checked...

SET(SHA1_FILES ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages/sphinx/cmdline.py)

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
                              ${RELATIVE_PROJECT_SOURCE_DIR} 7f459a2ee51e198caa5557a449d48d9515f1f7d3
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
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
        pip install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                    --upgrade --upgrade-strategy only-if-needed)

    # Install other packages we use

    ADD_CUSTOM_TARGET(other_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            sphinx==${SPHINX_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    ADD_DEPENDENCIES(${PACKAGE_BUILD} other_packages)
    ADD_DEPENDENCIES(other_packages ${BUILD_DEPENDENCIES})

    # Package the newly installed Python packages

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
        RELEASE_TAG ${RELEASE_TAG}
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
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    EXTERNAL_DESTINATION_DIR
        ${PYTHON_ROOT_DIR}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)

# Update Python paths for newly copied scripts

ADD_CUSTOM_COMMAND(TARGET PythonPackagesPlugin POST_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/distrib/bin/set_python_path.py ${PYTHON_ROOT_DIR})
