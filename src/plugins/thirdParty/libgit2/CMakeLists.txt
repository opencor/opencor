PROJECT(libgit2Plugin)

# Version of our package

SET(PACKAGE_VERSION "0.25.1")

# Version of our library

SET(MAJOR_LIBRARY_VERSION "0")
SET(LIBRARY_VERSION "${MAJOR_LIBRARY_VERSION}.25.1")

# Git tag for our library

SET(GIT_TAG "maint/v0.25")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY git2${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY git2${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
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

IF(USE_PREBUILT_LIBGIT2_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(libgit2 ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} de2a7191a23ad5c1f0477663eedc7eeffde5dc43
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 35bbdee04b20f196025803ff4ea7a352c230b56c
                            c0a1935fe1d5f002a5fa82db374fe3d7187b6b45
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(libgit2 ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 54e0d5d11ac0a79fad5b040426721a079136de6f
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 488549eecfac3e1bbf8fd79d5c525c88d94960be
                            c24a35a73ae006746c97f2ff128a60d950830f39
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(libgit2 ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} fe776e0524593f639e813275fc24e0e8ad8a2152
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES ccc41828d9356104b029964b888c559cfcaa75f2
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(libgit2 ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} e3f75a85e72b81183094e356c5df95997936f0cd
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 35057dd98a8eb4af28bce496141b75c207756cb3
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings

    IF(WIN32)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4267 /wd4311 /wd4312")
    ELSEIF(APPLE)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format")
    ENDIF()

    # Some specific arguments

    IF(WIN32)
        SET(STATIC_CRT_ARG "-DSTATIC_CRT=OFF")
    ELSE()
        SET(STATIC_CRT_ARG)
    ENDIF()

    # Build libgit2 as an external project

    SET(PACKAGE_BUILD libgit2Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libgit2.git
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_ARGS
            -DBUILD_CLAR=OFF
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            ${STATIC_CRT_ARG}
            -DUSE_SSH=OFF
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

        SET(FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY})

        IF(APPLE)
            SET(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}git2.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ELSE()
            SET(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}git2${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
        ENDIF()

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${REAL_FULL_SHARED_LIBRARY} ${FULL_SHARED_LIBRARY}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(libgit2 ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our libgit2 package

SET(LIBGIT2_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(LIBGIT2_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(LIBGIT2_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(LIBGIT2_PLUGIN libgit2 PARENT_SCOPE)

ADD_PLUGIN(libgit2
    SOURCES
        ../../plugininfo.cpp

        src/libgit2plugin.cpp
    HEADERS_MOC
        src/libgit2plugin.h
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
