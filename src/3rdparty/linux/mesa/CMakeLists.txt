project(Mesa)

# Name and version of our package

set(PACKAGE_NAME Mesa)
set(PACKAGE_VERSION 20.0.7)

# Git tag for our library

set(GIT_TAG v20.0.7)

# Specify where our local package will be installed

string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared library

set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}GL${CMAKE_SHARED_LIBRARY_SUFFIX}.1)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_MESA_PACKAGE)
    # Retrieve the library's package

    retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                          ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 048f7f231355caa810218066cee4062655d3a9c2
                          SHA1_FILES ${SHA1_FILES}
                          SHA1_VALUES ab0a4e49490c1454c9870279f94d56f0bf99a1b0)
else()
    # Build Mesa as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    set(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        DOWNLOAD_DIR
            ${PACKAGE_BUILD_DIR}
        SOURCE_DIR
            ${PACKAGE_BUILD_DIR}
        BINARY_DIR
            ${PACKAGE_BUILD_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/mesa
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ""
        BUILD_COMMAND
            scons force_scons=1 build=release libgl-xlib
        INSTALL_COMMAND
            cp ${PACKAGE_BUILD_DIR}/build/linux-x86_64/gallium/targets/libgl-xlib/${SHARED_LIBRARY} <INSTALL_DIR>/lib
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${SHARED_LIBRARY}
    )

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
endif()

# Allow our main CMake project to find our Mesa package

set(MESA_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(MESA_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY} PARENT_SCOPE)

# Copy our copy of the Mesa library

if(USE_PREBUILT_MESA_PACKAGE)
    set(COPY_TARGET DIRECT)
else()
    set(COPY_TARGET ${PACKAGE_BUILD})
endif()

copy_file_to_build_dir(${COPY_TARGET} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib lib ${SHARED_LIBRARY})
