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

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

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

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                          ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                          SHA1_FILES ${SHA1_FILES}
                          SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
else()
    # Build ICU as an external project

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
            https://github.com/opencor/icu4c
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            source/runConfigureICU Linux --prefix=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ICUDATA_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUI18N_SHARED_LIBRARY}
            <INSTALL_DIR>/lib/${ICUUC_SHARED_LIBRARY}
    )

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
endif()

# Allow our main CMake project to find our ICU package

set(ICU_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(ICU_ICUDATA_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ICUDATA_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_ICUI18N_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ICUI18N_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_ICUUC_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ICUUC_SHARED_LIBRARY} PARENT_SCOPE)
set(ICU_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${ICU_ICUDATA_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${ICU_ICUI18N_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${ICU_ICUUC_LIBRARY}
    PARENT_SCOPE
)

# Copy our copy of the ICU library

if(USE_PREBUILT_ICU_PACKAGE)
    set(COPY_TARGET DIRECT)
else()
    set(COPY_TARGET ${PACKAGE_BUILD})
endif()

copy_file_to_build_dir(${COPY_TARGET} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib lib ${ICU_ICUDATA_LIBRARY})
copy_file_to_build_dir(${COPY_TARGET} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib lib ${ICU_ICUI18N_LIBRARY})
copy_file_to_build_dir(${COPY_TARGET} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib lib ${ICU_ICUUC_LIBRARY})
