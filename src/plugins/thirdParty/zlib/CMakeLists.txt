PROJECT(zlibPlugin)

# Version that we want to use

SET(ZLIB_VERSION "1.2.11")
SET(ZLIB_MAJOR_VERSION "1")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    IF(RELEASE_MODE)
        SET(ZLIB_SHARED_LIBRARY zlib${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_IMPORT_LIBRARY zlib${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSE()
        SET(ZLIB_SHARED_LIBRARY zlibd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_IMPORT_LIBRARY zlibd${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ENDIF()
ELSEIF(APPLE)
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_MAJOR_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ZLIB_IMPORT_LIBRARY ${ZLIB_SHARED_LIBRARY})
ELSE()
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_MAJOR_VERSION})
    SET(ZLIB_IMPORT_LIBRARY ${ZLIB_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${ZLIB_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES lib/${ZLIB_SHARED_LIBRARY})

IF(WIN32)
    LIST(APPEND SHA1_FILES lib/${ZLIB_IMPORT_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 40a096109698a815d48207a85d0afdecdb7d0547
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 47a71185f4c448eda5ef74a680245043e802e80b
                            5f16a53aeff8f826616adefe03d170a641d40e44
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 910865644cf82b247692e9834a936f88a5990fd6
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 5a4692ed213caf95c43aa1e4a7499f5925a61a1f
                            e24f37994183f17689f8cb7073b53a4741d6b286
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 6788c9fb79e0c9f9a8f85d77aab357e5ba0b8128
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 2da040b5883c4a00166e9a22cc49c5400d09d025
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} dac6e090b9848569bade27c9e860c7fe359bce5f
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES e7a94eaf5cd6bfa099ffc700ea81f26e3030a065
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    # Build zlib as an external project

    ExternalProject_Add(zlibBuild
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/zlib.git
        GIT_TAG
            v${ZLIB_VERSION}
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ZLIB_IMPORT_LIBRARY}
    )

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(zlibBuild CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/${ZLIB_SHARED_LIBRARY}
                                                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        IF(APPLE)
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
            )
        ELSE()
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_VERSION}
            )
        ENDIF()

        ExternalProject_Add_Step(zlibBuild CopyLibrary
            COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ACTUAL_ZLIB_SHARED_LIBRARY}
                                             ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            DEPENDEES install)
    ENDIF()

    # Package zlib's include files and library

    CREATE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES include ${SHA1_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET zlibBuild
    )

    SET(PLUGIN_DEPENDS_ON zlibBuild)
ENDIF()

# Allow other external projects to find our zlib

SET(ZLIB_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(ZLIB_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(ZLIB_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(ZLIB_PLUGIN zlib PARENT_SCOPE)

ADD_PLUGIN(zlib
    SOURCES
        ../../plugininfo.cpp
        src/zlibplugin.cpp
    HEADERS_MOC
        src/zlibplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)
