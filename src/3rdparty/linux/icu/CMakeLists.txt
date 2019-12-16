project(ICU)

# Name and version of our package

set(PACKAGE_NAME ICU)
set(PACKAGE_VERSION 56.1)

# Version of our library

set(MAJOR_LIBRARY_VERSION 56)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1)

# Git tag for our library

set(GIT_TAG v56.1)

# Specify where our local package will be installed

string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared library

set(ICUDATA_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icudata${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
set(ICUI18N_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icui18n${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
set(ICUUC_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}icuuc${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${ICUDATA_SHARED_LIBRARY}
    lib/${ICUI18N_SHARED_LIBRARY}
    lib/${ICUUC_SHARED_LIBRARY}
)

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_ICU_PACKAGE)
    # Retrieve the library's package

    retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                          ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 37cf2e70a0e2aa3f3814360e182a45269c7001f6
                          SHA1_FILES ${SHA1_FILES}
                          SHA1_VALUES 09c48c1e7ecaee2ce66d2214eb791bf14a58ed9c
                                      8ce5cb893e638bc02060271c0fc2f7cbf0156590
                                      34be5958abf7a7af5f624893925b60cf069f7c43)
else()
    # Build ICU as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        BINARY_DIR
            ${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/icu
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            <SOURCE_DIR>/source/runConfigureICU Linux --prefix=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ICUDATA_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUI18N_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUUC_SHARED_LIBRARY}
    )

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES include lib
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
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
    linux_deploy_qt_library(${DEPLOY_TARGET} ${LOCAL_ICU_LIB_DIR} ${ICU_LIBRARY})
endforeach()
