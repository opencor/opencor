project(Mesa)

# Our package characteristics

set(PACKAGE_NAME Mesa)
set(PACKAGE_VERSION 22.1.0)
set(PACKAGE_REPOSITORY mesa)
set(GIT_TAG v22.1.0)
set(RELEASE_TAG v22.1.0)

get_full_local_external_package_dir()

# Name of our package's shared library

set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}GL${CMAKE_SHARED_LIBRARY_SUFFIX}.1)

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${SHARED_LIBRARY}
)

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_MESA_PACKAGE)
    # Retrieve the library's package

    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                     b3264307939fd0c244fa41a536b0ba7be5b5ba99)
else()
    # Build Mesa as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        BINARY_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CONFIGURE_COMMAND
            meson build -Dbuildtype=release -Ddri-drivers= -Ddri3=disabled -Dgallium-drivers=swrast -Dglx=auto -Dllvm=disabled -Doptimization=3 -Dplatforms=x11 -Dvulkan-drivers=
        BUILD_COMMAND
            cd build && ninja
        INSTALL_COMMAND
            cp build/src/gallium/targets/libgl-xlib/${SHARED_LIBRARY} <INSTALL_DIR>/lib
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${SHARED_LIBRARY}
    )

    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   ${STRIPPABLE_FILES})
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
