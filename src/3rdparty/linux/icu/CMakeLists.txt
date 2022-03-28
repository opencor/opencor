project(ICU)

# Our package characteristics

set(PACKAGE_NAME ICU)
set(PACKAGE_VERSION 56.1)
set(PACKAGE_REPOSITORY icu)
set(GIT_TAG v56.1)
set(RELEASE_TAG v56.1)

get_full_local_external_package_dir()

# Version of our library

set(MAJOR_LIBRARY_VERSION 56)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1)

# Name of our package's shared library

set(ICUDATA_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icudata${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
set(ICUI18N_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icui18n${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
set(ICUUC_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icuuc${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${ICUDATA_SHARED_LIBRARY}
    lib/${ICUI18N_SHARED_LIBRARY}
    lib/${ICUUC_SHARED_LIBRARY}
)

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_ICU_PACKAGE)
    # Retrieve the library's package

    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                     37cf2e70a0e2aa3f3814360e182a45269c7001f6)
else()
    # Build ICU as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CONFIGURE_COMMAND
            <SOURCE_DIR>/source/runConfigureICU Linux --prefix=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ICUDATA_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUI18N_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUUC_SHARED_LIBRARY}
    )

    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})
endif()

# Allow our main CMake project to find our ICU package

set(LOCAL_ICU_LIB_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(ICU_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(ICU_ICUDATA_LIBRARY ${LOCAL_ICU_LIB_DIR}/${ICUDATA_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_ICUI18N_LIBRARY ${LOCAL_ICU_LIB_DIR}/${ICUI18N_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_ICUUC_LIBRARY ${LOCAL_ICU_LIB_DIR}/${ICUUC_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_LIBRARIES
    ${LOCAL_ICU_LIB_DIR}/${ICUDATA_SHARED_LIBRARY}
    ${LOCAL_ICU_LIB_DIR}/${ICUI18N_SHARED_LIBRARY}
    ${LOCAL_ICU_LIB_DIR}/${ICUUC_SHARED_LIBRARY}
    PARENT_SCOPE
)
set(ICU_PKGCONFIG_DIR ${LOCAL_ICU_LIB_DIR}/pkgconfig PARENT_SCOPE)

# (Copy and) deploy our copy of the ICU libraries

if(USE_PREBUILT_ICU_PACKAGE)
    set(DEPLOY_TARGET DIRECT)
else()
    set(DEPLOY_TARGET ${PACKAGE_BUILD})
endif()

foreach(ICU_LIBRARY ${ICUDATA_SHARED_LIBRARY} ${ICUI18N_SHARED_LIBRARY} ${ICUUC_SHARED_LIBRARY})
    linux_deploy_binary_file(${DEPLOY_TARGET} ${LOCAL_ICU_LIB_DIR} lib ${ICU_LIBRARY})
endforeach()
