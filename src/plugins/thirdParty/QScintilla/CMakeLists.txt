project(QScintillaPlugin)

# Name and version of our package

set(PACKAGE_NAME QScintilla)
set(PACKAGE_VERSION 2.11.6)

# Version of our library

set(MAJOR_LIBRARY_VERSION 15)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0.0)

# Git tag for our library

set(GIT_TAG v2.11.6)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    if(RELEASE_MODE)
        set(DEBUG_TAG)
    else()
        set(DEBUG_TAG d)
    endif()

    set(SHARED_LIBRARY qscintilla2_qt${QT_VERSION_MAJOR}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY qscintilla2_qt${QT_VERSION_MAJOR}${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()

# Determine whether we should strip the library

if(NOT WIN32 AND NOT APPLE)
    set(NO_STRIP NO_STRIP)
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QSCINTILLA_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 81998fa0ae79174ee46ae407259f71833e063d5c
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 68c226988931b88333dc4ce076b7b573e46e816b
                                              1c253a60134481292b9ea508c79ca42b2808f8e0)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} d9125662875f47675539ef0bbd76ed48a12456c7
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 248466d735dceb87e8c56f532795a1eb46b8f777
                                              62af84b7b5119516b6a804c5cce55457087bd514)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 23400baf34f48d68236a5c53de21199978532cfd
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES edc99e05796941b7d5ba949b655224d7e1aae45b)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 831204f5747e1e831d1670d0025f53b447784602
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d91f27b0f9bcacd86e771cc56fee3d8714425ad4)
    endif()
else()
    # Set platform specific configuration options

    if(WIN32)
        if(RELEASE_MODE)
            set(MAKEFILE /f Makefile.Release)
        else()
            set(MAKEFILE /f Makefile.Debug)
        endif()
    else()
        set(MAKEFILE -f Makefile)
    endif()

    # Build QScintilla as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qscintilla
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} <SOURCE_DIR>/Qt4Qt5 OBJECTS_DIR=${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_JOM_COMMAND} ${MAKEFILE}
        INSTALL_COMMAND
            ${MAKE_JOM_COMMAND} ${MAKEFILE} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        # Fix the RPATH information

        if(APPLE)
            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARIES_DIR} ${SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES copyLibrary)
        else()
            # Note: we strip the library here (rather than through
            #       create_package() and add_plugin(), hence we pass NO_STRIP to
            #       them) due to an issue with the system version of patchelf,
            #       which prevents the library from being safely stripped after
            #       being patched...

            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND strip -x ${SHARED_LIBRARY}
                                     COMMAND patchelf --remove-rpath ${SHARED_LIBRARY}
                                     COMMAND patchelf --set-rpath $ORIGIN --force-rpath ${SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES copyLibrary)
        endif()
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD}
                        ${NO_STRIP})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our QScintilla package

set(QSCINTILLA_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(QSCINTILLA_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(QSCINTILLA_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qscintillaplugin.cpp
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    ${NO_STRIP}
    DEPENDS_ON
        ${DEPENDS_ON}
)
