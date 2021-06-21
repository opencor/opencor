project(QwtPlugin)

# Name and version of our package

set(PACKAGE_NAME Qwt)
set(PACKAGE_VERSION 6.1.6)

# Version of our library

set(MAJOR_LIBRARY_VERSION 6)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.6)

# Git tag for our library

set(GIT_TAG v6.1.6)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    if(RELEASE_MODE)
        set(DEBUG_TAG)
    else()
        set(DEBUG_TAG d)
    endif()

    set(QWT_SHARED_LIBRARY qwt${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWT_IMPORT_LIBRARY qwt${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(QWTMATHML_SHARED_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWTMATHML_IMPORT_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(QWT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${QWT_SHARED_LIBRARY}
    lib/${QWTMATHML_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
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
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 5c525e04ca0597dd3ac90059b03d68f981be75a9
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES c3e3bf3184cac0fe5d05f9a2bab44b6034d702da
                                              5d4a990e21c799c10d44d0d162d2dc599fce209d
                                              4d04ecbc6c1ea50d4fc7ae8094af9d5f97706064
                                              ab7fc33185dd02cc9e6f39daf87b49b23b72fbaa)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} e9fee7a8e8bec8b42b24875be1cc9e18e452c7f6
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 286c84aa5a76656345ce49474b6500719faffa7f
                                              c95238d47fb42144251dd331d6ca71fa164533b1
                                              58bdc94d8f1c531a3d324bf2c7e69a3b2362d0f5
                                              d3c451f84b79b2479dc8165475e739c5653835e6)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} da96c14e1f0cb45fddafe1aede1e01b56dc98eea
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 07877f6eef44bd668437c899923c66441288a85c
                                          6c97bf61a498e1f54f1d9fe330e3d185b6a29c01)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 3f7a69ba895b9663478180c220e7e5fb0835c486
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 0cee985445814100cc627396276b16d3f882e4aa
                                          d4d1f0bc0eed3009ca45f919456480fcdce43b9b)
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
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qwt
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} <SOURCE_DIR> CONFIG+=qt_no_framework OBJECTS_DIR=${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
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

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD}
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
