project(PythonPackagesPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION ${PYTHON_VERSION})

# Package repository and release tag

set(PACKAGE_REPOSITORY python)
set(RELEASE_TAG v${PYTHON_VERSION}_base)

# The versions of the Python packages we bundle

set(SPHINX_VERSION 1.7.2)

# Our GitHub organisation

set(GITHUB_OPENCOR https://github.com/opencor)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

set(WHEEL_REPOSITORY ${GITHUB_OPENCOR}/python/releases/download/wheels)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)
set(FULL_LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LOCAL_SITE_PACKAGES_DIR})

# Directories we package

set(PACKAGED_FILES
    ${LOCAL_SITE_PACKAGES_DIR}
    ${PYTHON_RELATIVE_SCRIPT_DIR}
)

# Specify the files that need to have their SHA-1 value checked
# Note: at least one file must have its SHA-1 value checked...

set(SHA1_FILES
    ${LOCAL_SITE_PACKAGES_DIR}/sphinx/cmdline.py
)

# Use the pre-built version of our package unless instructed otherwise
# Rebuilding Python means we should also re-install its packages

if(USE_PREBUILT_PYTHON_PACKAGE AND USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 9f950862b63ccf386e47be21a8a2da3c68687b9f
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
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
                              ${RELATIVE_PROJECT_SOURCE_DIR} de19e8a6d17b8908c2e396fde5252e5dab0a050f
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 2bdb262c6599dc8dfd13a592baa580c621b2839c
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
    endif()
else()
    # Install Python packages using pip

    set(PACKAGE_BUILD PythonPackagesBuild)

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

    # Install Sphinx

    add_custom_target(installSphinx
                      COMMAND ${PIP_INSTALL_COMMAND} --only-binary all sphinx==${SPHINX_VERSION}
                      WORKING_DIRECTORY ${FULL_DEST_EXTERNAL_BINARIES_DIR}
                      VERBATIM)

    add_dependencies(${PACKAGE_BUILD} installSphinx)
    add_dependencies(installSphinx ${BUILD_DEPENDENCIES})

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
    set(SPHINX_BUILD_EXECUTABLE sphinx-build.exe)
else()
    set(SPHINX_BUILD_EXECUTABLE sphinx-build)
endif()

set(SPHINX_BUILD_EXECUTABLE
    ${CMAKE_COMMAND} -E env "PYTHONPATH=${FULL_LOCAL_SITE_PACKAGES_DIR}"
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_SCRIPT_DIR}/${SPHINX_BUILD_EXECUTABLE} PARENT_SCOPE
)

# We are not relying on add_plugin() to build our Python site packages, so we
# need to specify some dependencies

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Update the Python path in newly copied scripts

add_custom_command(TARGET ${PROJECT_NAME}
                   COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_SCRIPT_DIR}/set_python_path.py
                                                --update-path ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_SCRIPT_DIR}
                                                ${PYTHON_ROOT_DIR} -s)
