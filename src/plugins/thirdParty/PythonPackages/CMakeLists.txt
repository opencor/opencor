project(PythonPackagesPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonPackages)
set(PACKAGE_VERSION ${PYTHON_VERSION})
set(RELEASE_TAG v${PYTHON_VERSION}_base)

# The repository for our package

set(PACKAGE_REPOSITORY python)

# The versions of the Python packages we bundle.

set(SPHINX_VERSION 1.7.2)

# Our GitHub archive

set(GITHUB_OPENCOR https://github.com/opencor)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

set(WHEEL_REPOSITORY ${GITHUB_OPENCOR}/python/releases/download/wheels)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Where Python packages are installed

set(LOCAL_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages)

# Package the `bin`, `include` and `lib` directories

set(PACKAGED_FILES
    bin
    ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages
)

# At least one file must have its SHA-1 value checked...

set(SHA1_FILES ${PYTHON_RELATIVE_RUNTIME_DIR}/site-packages/sphinx/cmdline.py)

# Use the pre-built version of our package unless instructed otherwise
# Rebuilding Python means we should also re-install its packages

if(USE_PREBUILT_PYTHON_PACKAGE AND USE_PREBUILT_PYTHON_PACKAGES_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 47119dd449b75490a90c18d8cc71945af080247f
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 97949f3f839adab391fc785d8f7459f569f63a4f)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 7f459a2ee51e198caa5557a449d48d9515f1f7d3
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3483da0d92f6b154ff7ce699350e741da0a84104)
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
        pip install --prefix ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                    --upgrade --upgrade-strategy only-if-needed)

    # Install other packages we use

    add_custom_target(other_packages
        COMMAND ${PIP_INSTALL_COMMAND} --only-binary all
            sphinx==${SPHINX_VERSION}
        WORKING_DIRECTORY
            ${FULL_DEST_EXTERNAL_BINARIES_DIR}
        VERBATIM
    )

    add_dependencies(${PACKAGE_BUILD} other_packages)
    add_dependencies(other_packages ${BUILD_DEPENDENCIES})

    # Package the newly installed Python packages

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
        RELEASE_TAG ${RELEASE_TAG}
        PACKAGED_FILES ${PACKAGED_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
        )

    set(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other packages to use our scripts

if(WIN32)
    set(SPHINX_BUILD_EXECUTABLE sphinx-build.exe)
else()
    set(SPHINX_BUILD_EXECUTABLE sphinx-build)
endif()

set(SPHINX_BUILD_EXECUTABLE ${PYTHON_ROOT_DIR}/bin/${SPHINX_BUILD_EXECUTABLE} PARENT_SCOPE)

# Add the plugin

add_plugin(PythonPackages
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

add_custom_command(TARGET PythonPackagesPlugin POST_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/distrib/bin/set_python_path.py ${PYTHON_ROOT_DIR})
