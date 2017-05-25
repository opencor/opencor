PROJECT(Mesa)

# Name and version of our package

SET(PACKAGE_NAME Mesa)
SET(PACKAGE_VERSION 17.1.0)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 1)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.5)

# Git tag for our library

SET(GIT_TAG v17.1.0)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library

SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}GL${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_MESA_PACKAGE)
    # Retrieve the library's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                          ${RELATIVE_PROJECT_SOURCE_DIR} 348994f84ad35c193f498279667977b4830de560
                          SHA1_FILES ${SHA1_FILES}
                          SHA1_VALUES d334e3b4d40851c6d4d71043ed8bdd3444d04583)
ELSE()
    # Build Mesa as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    SET(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

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
            scons build=release libgl-xlib
        INSTALL_COMMAND
            cp ${PACKAGE_BUILD_DIR}/build/linux-x86_64/gallium/targets/libgl-xlib/${SHARED_LIBRARY} <INSTALL_DIR>/lib
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${SHARED_LIBRARY}
    )

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
ENDIF()

# Allow our main CMake project to find our Mesa package

SET(MESA_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(MESA_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY} PARENT_SCOPE)

# Copy our copy of the Mesa library

IF(USE_PREBUILT_MESA_PACKAGE)
    SET(COPY_TARGET DIRECT)
ELSE()
    SET(COPY_TARGET ${PACKAGE_BUILD})
ENDIF()

COPY_FILE_TO_BUILD_DIR(${COPY_TARGET} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib lib ${SHARED_LIBRARY})
