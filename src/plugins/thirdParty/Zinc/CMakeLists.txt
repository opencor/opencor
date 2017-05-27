PROJECT(ZincPlugin)

# Name and version of our package

SET(PACKAGE_NAME Zinc)
SET(PACKAGE_VERSION 3.1.1)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 3)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.1)

# Git tag for our library

SET(GIT_TAG v3.1.1)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    IF(NOT RELEASE_MODE)
        SET(DEBUG_TAG d)
    ENDIF()

    SET(SHARED_LIBRARY zinc${DEBUG_TAG}.${LIBRARY_VERSION}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY zinc${DEBUG_TAG}.${LIBRARY_VERSION}${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}zinc${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

IF(USE_PREBUILT_ZINC_PACKAGE)
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
                              ${RELATIVE_PROJECT_SOURCE_DIR} 80e42af66c2ea2d65fc76590fe76b16362cb136d
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 38cdefca68fe8ff4e5d81ba36cf738dfdd9e1417)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 82075e69cc3b524647e31603d08290286785d9c8
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 7935dd45b285c1e8b886d305b6dd12075f0b4bb0)
    ENDIF()
ELSE()
    # Set platform specific configuration options

    IF(WIN32)
        MATH(EXPR MSVC_VERSION "${CMAKE_CXX_COMPILER_VERSION_MAJOR}-4")

        SET(DEFINE_CMAKE_GENERATOR -G "Visual Studio 14 20${MSVC_VERSION} Win64")

        SET(SHARED_lIBRARY_DIR bin)
    ELSE()
        SET(SHARED_lIBRARY_DIR lib)
    ENDIF()

    # Build Zinc as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}/setup
        BINARY_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}/setup-build
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/zinc
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${CMAKE_COMMAND} ${DEFINE_CMAKE_GENERATOR} ${DEFINE_CMAKE_MACOSX_RPATH} -DOPENCMISS_CONFIG_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DOPENCMISS_ROOT=<BINARY_DIR>/.. -DOPENCMISS_LIBRARIES=zinc <SOURCE_DIR>
        BUILD_COMMAND
            ${CMAKE_COMMAND} --build .
        INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/../install/include/opencmiss <INSTALL_DIR>/include/opencmiss
            && ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/../install/${SHARED_lIBRARY_DIR}/${SHARED_LIBRARY} <INSTALL_DIR>/lib
            && ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/../install/lib/${IMPORT_LIBRARY} <INSTALL_DIR>/lib
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

# Allow other external projects to find our Zinc package

SET(ZINC_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(ZINC_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(ZINC_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/zincplugin.cpp
    HEADERS_MOC
        src/zincplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    TESTS
        tests
    DEPENDS_ON
        ${DEPENDS_ON}
)
