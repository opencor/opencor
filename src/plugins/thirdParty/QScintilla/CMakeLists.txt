project(QScintillaPlugin)

# Name and version of our package

set(PACKAGE_NAME QScintilla)
set(PACKAGE_VERSION 2.10.3)

# Version of our library

set(MAJOR_LIBRARY_VERSION 13)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.1)

# Git tag for our library

set(GIT_TAG v2.10.3)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

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

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QSCINTILLA_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 20a0987c5f42b2a8dc4ded6ce41f3eb130d1b53a
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 4852c891bd90a5993f6df221a36e0c841d82c378
                                              9a9cd12bdaa3e3897d091a4ed7a13b115f80e69f)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} beca81017f4dbe95b47a9b72da36e712f817b480
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 5f9fbe34cf1d226fa4f0827c61459412022990ca
                                              5ed37e6c582cf936baf8eac429601ffb8ba24bf8)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 31c55ce6dea6543c97a4af10d3882dd734f78c61
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 5d88f66453fd8bbbd8ef24ee796eed6ac3c98efa)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 43756aeb69b6401c8b7146a4d0f0525c9af4d690
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d19dac33970c5bebc7f3fdb7349a83875eca4c41)
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

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        if(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES CopyLibrary)
        endif()
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

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
    HEADERS_MOC
        src/qscintillaplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
