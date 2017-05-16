PROJECT(OAuthPlugin)

# Name and version of our package

SET(PACKAGE_NAME o2)
SET(PACKAGE_VERSION "1.0.1")

# Version of our library

SET(REAL_LIBRARY_VERSION "1.0.1")
SET(USED_LIBRARY_VERSION "1.0.0")

# Git tag for our library

SET(GIT_TAG "v1.0.1")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY o2${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY o2${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}o2.${USED_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}o2${CMAKE_SHARED_LIBRARY_SUFFIX}.${USED_LIBRARY_VERSION})
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

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_OAUTH_PACKAGE)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 99aeb4090b82371a7f53a050ee4101754ae0c121
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES a0581cce0eb11569c86b9ffeae3992630d812d8f
                            dd115af5184fb5e4b8d72097bd5061b450f76607
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} a5db362b9cd56bc7cb8a8d5027471f0683a8ba45
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 952f48c45390daf88792402cd7ebd5689777a72f
                            cab5bd90837992be5ff80fd4a548002b0a93694a
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 4a77f0c73e95c39b817b9a4e02a9873e25606a7e
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES ca1353027b5d4199de22948f24c07c3e5329eeb4
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 842c015bb7ac2969145ef52e778370990c03cc93
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 297f187a580f6489ed64223df93ebf4639efeceb
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings

    IF(WIN32)
        SET(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4061 /wd4127 /wd4242 /wd4365 /wd4464 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4710 /wd4711 /wd4820 /wd5026  /wd5027")
    ENDIF()

    # Build OAuth as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/o2
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_CXX_FLAGS:STRING=${PACKAGE_CXX_FLAGS}
        CMAKE_ARGS
            -DBUILD_SHARED_LIBS=ON
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            -Do2_WITH_OAUTH1=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        SET(FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}o2${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${REAL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${SHARED_LIBRARY}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our OAuth package

SET(OAUTH_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(OAUTH_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(OAUTH_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin
# Note: we would normally use '${PACKAGE_NAME}' rather than 'OAuth', but it
#       can't be done here since the name of our package is different from that
#       of our plugin...

SET(OAUTH_PLUGIN OAuth PARENT_SCOPE)

ADD_PLUGIN(OAuth
    SOURCES
        ../../plugininfo.cpp

        src/oauthplugin.cpp
    HEADERS_MOC
        src/oauthplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include

        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
