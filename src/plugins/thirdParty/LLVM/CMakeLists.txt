PROJECT(LLVMPlugin)

# Name and version of our package

SET(PACKAGE_NAME LLVM)
SET(PACKAGE_VERSION 4.0.0)

# Version of our library

SET(LIBRARY_VERSION 4.0.0)

# Git tag for our library

SET(GIT_TAG v4.0.0)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY llvm${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY llvm${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}llvm${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_LLVM_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ENDIF()
ELSE()
    # Build LLVM as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/llvm
        GIT_TAG
            ${GIT_TAG}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our LLVM package

SET(LLVM_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(LLVM_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(LLVM_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS_MOC
        src/llvmplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
