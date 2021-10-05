project(Mesa)

# Name and version of our package

set(PACKAGE_NAME Mesa)
set(PACKAGE_VERSION 21.2.3)

# Git tag for our library

set(GIT_TAG v21.2.3)

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
                          ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 0205e25366e04bae4aa53fe80c462398e73c8f6e
                          SHA1_FILES ${SHA1_FILES}
                          SHA1_VALUES a3c2cb7cff4df0f3c5bf0dbeff2cf78b3e5516b5)
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
            meson build -Dbuildtype=release -Ddri-drivers= -Ddri3=disabled -Dgallium-drivers=swrast -Dglx=gallium-xlib -Dllvm=disabled -Doptimization=3 -Dplatforms=x11 -Dvulkan-drivers=
        BUILD_COMMAND
            cd build && ninja
        INSTALL_COMMAND
            cp build/src/gallium/targets/libgl-xlib/${SHARED_LIBRARY} <INSTALL_DIR>/lib
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
