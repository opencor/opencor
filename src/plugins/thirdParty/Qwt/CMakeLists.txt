project(QwtPlugin)

# Our package characteristics

set(PACKAGE_NAME Qwt)
set(PACKAGE_VERSION 6.2.0)
set(PACKAGE_REPOSITORY qwt)
set(GIT_TAG v6.2.0)
set(RELEASE_TAG v6.2.0)

get_full_local_external_package_dir()

# Version of our package's library

set(MAJOR_LIBRARY_VERSION 6)
set(MINOR_LIBRARY_VERSION 2)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(QWT_SHARED_LIBRARY qwt${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWT_IMPORT_LIBRARY qwt${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(QWTMATHML_SHARED_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWTMATHML_IMPORT_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
elseif(APPLE)
    set(QWT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWT_IMPORT_LIBRARY ${QWT_SHARED_LIBRARY})

    set(QWTMATHML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWTMATHML_IMPORT_LIBRARY ${QWTMATHML_SHARED_LIBRARY})
else()
    set(QWT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWT_IMPORT_LIBRARY ${QWT_SHARED_LIBRARY})

    set(QWTMATHML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWTMATHML_IMPORT_LIBRARY ${QWTMATHML_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${QWT_SHARED_LIBRARY}
    ${QWTMATHML_SHARED_LIBRARY}
)

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${QWT_SHARED_LIBRARY}
    lib/${QWTMATHML_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        lib/${QWT_IMPORT_LIBRARY}
        lib/${QWTMATHML_IMPORT_LIBRARY}
    )
endif()

# Determine whether we should strip the libraries

if(NOT WIN32 AND NOT APPLE)
    set(NO_STRIP NO_STRIP)
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QWT_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             c0511b71ee60db94d75f87f5045f9e17078e9026)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             e9d7a1d68ebbe208558df0881d1944a06e8931ed)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         b35136ca456e4357ffaf219eb1ec3b3e5562740f)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         204a583b3c535dca60fd5d7ca0ecb8e8e1e5282d)
    endif()
else()
    # Set platform specific configuration options

    if(WIN32)
        if(RELEASE_MODE)
            set(MAKEFILE_TARGET release)
        else()
            set(MAKEFILE_TARGET debug)
        endif()
    endif()

    # Build Qwt as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} <SOURCE_DIR> CONFIG+=qt_no_framework INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_JOM_COMMAND} ${MAKEFILE_TARGET}
        INSTALL_COMMAND
            ${MAKE_JOM_COMMAND} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${QWT_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${QWTMATHML_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyQwtLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyQwtmathmlLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWTMATHML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWTMATHML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        # Fix the RPATH information

        if(APPLE)
            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARIES_DIR} ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARIES_DIR} ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${QWT_SHARED_LIBRARY}
                                                                       @rpath/${QWT_SHARED_LIBRARY}
                                                                       ${QWTMATHML_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES copyQwtmathmlLibrary)
        else()
            # Note: we strip the libraries here (rather than through
            #       create_package() and add_plugin(), hence we pass NO_STRIP to
            #       them) due to an issue with the system version of patchelf,
            #       which prevents the libraries from being safely stripped
            #       after being patched...

            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND strip -x ${QWT_SHARED_LIBRARY}
                                     COMMAND patchelf --remove-rpath ${QWT_SHARED_LIBRARY}
                                     COMMAND patchelf --set-rpath $ORIGIN --force-rpath ${QWT_SHARED_LIBRARY}
                                     COMMAND strip -x ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND patchelf --remove-rpath ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND patchelf --set-rpath $ORIGIN --force-rpath ${QWTMATHML_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES copyQwtmathmlLibrary)
        endif()
    endif()

    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES}
                   ${NO_STRIP})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our Qwt package

set(QWT_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(QWT_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(QWT_QWT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY} PARENT_SCOPE)
set(QWT_QWTMATHML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY} PARENT_SCOPE)
set(QWT_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qwtplugin.cpp
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
