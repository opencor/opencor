project(QwtPlugin)

# Name and version of our package

set(PACKAGE_NAME Qwt)
set(PACKAGE_VERSION 6.1.3)

# Version of our library

set(MAJOR_LIBRARY_VERSION 6)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.3)

# Git tag for our library

set(GIT_TAG v6.1.3)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

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

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QWT_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 26d0408acc800d2254359cd08f54e3316a40d854
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 48d1ff3f27a5c474ea9140db3f967ee59e1c013b
                                              77c9ffad27d8970571508f479d1a944a49c6be70
                                              58a84a8e3b3dee4eae7243b86d1ae71880201abb
                                              e7c24127e29420a553cd5143bdf8a9d01ab58d5a)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b660184b7183c9c77065ae312e836c63220036c1
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 797be5b8c8274f5d79dba26d0c8cfc81d37def55
                                              1a11e5972ba5cbe929f00aab8bc683aa5f32b220
                                              50a3c9853ef6e732de0bd08e982f5da2f91f3c03
                                              282f4e4f017b7abdf224a2b1460e766647a06277)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1168208c7d280bc622ef850d98e2f045c8a30759
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 192399896b63528a83a481df6d2ce6b690b64c0f
                                          7b1c5bf136a8639197cddef53c76c7661f397260)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 52592dbd18c64155408cd0411144cdf48f041e42
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES dfc29b02b23434e793968332a95ea5fe5357f90e
                                          94183cb96c5b1f93ee65bca9ce19d7f0273ec650)
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

        if(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${QWT_SHARED_LIBRARY}
                                                                       @rpath/${QWT_SHARED_LIBRARY}
                                                                       ${QWTMATHML_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES copyQwtmathmlLibrary)
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
    HEADERS_MOC
        src/qwtplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
