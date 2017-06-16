PROJECT(QtWebKit)

# Name and version of our package

SET(PACKAGE_NAME QtWebKit)
SET(PACKAGE_VERSION 5.6.2)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 5)
SET(MINOR_LIBRARY_VERSION 6)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.2)

# Git tag for our library

SET(GIT_TAG v5.6.2)

# Platform target for our library

SET(TARGET_PLATFORM ${PLATFORM})

# Specify where our local package will be installed
# Note: on Windows, we would normally distinguish between release and debug 
#       modes, but QtWebKit's build system doesn't, hence we go one folder up...

SET(REAL_LOCAL_EXTERNAL_PACKAGE_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR}/..)
SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${REAL_LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(QTWEBKIT_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKIT_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${CMAKE_IMPORT_LIBRARY_SUFFIX})
    SET(QTWEBKITD_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKitd${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKITD_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKitd${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(QTWEBKITWIDGETS_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_IMPORT_LIBRARY_SUFFIX})
    SET(QTWEBKITWIDGETSD_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgetsd${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKITWIDGETSD_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgetsd${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(QTWEBKIT_DIR QtWebKit.framework)
    SET(QTWEBKIT_SHARED_LIBRARY_DIR ${QTWEBKIT_DIR}/Versions/${QT_VERSION_MAJOR})
    SET(QTWEBKIT_SHARED_LIBRARY ${QTWEBKIT_SHARED_LIBRARY_DIR}/QtWebKit)
    SET(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    SET(QTWEBKITWIDGETS_DIR QtWebKitWidgets.framework)
    SET(QTWEBKITWIDGETS_SHARED_LIBRARY_DIR ${QTWEBKITWIDGETS_DIR}/Versions/${QT_VERSION_MAJOR})
    SET(QTWEBKITWIDGETS_SHARED_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY_DIR}/QtWebKitWidgets)
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
ELSE()
    SET(BASE_QTWEBKIT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKIT_SHARED_LIBRARY ${BASE_QTWEBKIT_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION})
    SET(REAL_QTWEBKIT_SHARED_LIBRARY ${BASE_QTWEBKIT_SHARED_LIBRARY}.${LIBRARY_VERSION})
    SET(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    SET(BASE_QTWEBKITWIDGETS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKITWIDGETS_SHARED_LIBRARY ${BASE_QTWEBKITWIDGETS_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION})
    SET(REAL_QTWEBKITWIDGETS_SHARED_LIBRARY ${BASE_QTWEBKITWIDGETS_SHARED_LIBRARY}.${LIBRARY_VERSION})
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
ENDIF()

# Specify the files that need to have their SHA-1 value checked

IF(WIN32)
    SET(SHARED_LIBRARIES_DIR bin)
ELSE()
    SET(SHARED_LIBRARIES_DIR lib)
ENDIF()

SET(SHA1_FILES
    ${SHARED_LIBRARIES_DIR}/${QTWEBKIT_SHARED_LIBRARY}
    ${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        ${SHARED_LIBRARIES_DIR}/${QTWEBKITD_SHARED_LIBRARY}
        ${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETSD_SHARED_LIBRARY}

        lib/${QTWEBKIT_IMPORT_LIBRARY}
        lib/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
        lib/${QTWEBKITD_IMPORT_LIBRARY}
        lib/${QTWEBKITWIDGETSD_IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    IF(WIN32)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 72c4b5eb31c7b765902f2626ed1235f839a8e9f3
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES fc201490aad70770d07787703628a1725a207008
                                          4d179cc6be2208d384d281b3fbf4cc909081fec9
                                          a7c0049c1f9e30789ce6297ef89ad9b4d2fcc7a5
                                          0862f2508611479a9afb99b87a5a22b070903b78
                                          b87c17a223f04415a231071ba5b1f26251fc58fc
                                          94df3dd152587f29d001f8d0f29a7d393bbd0887
                                          3b7d2ac14dde12de9f13c215002322e0e05bf8c3
                                          6862b151d04b007648eb3be069c3a7dc22393bf1)
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} b94d50c37b9be59c34c991b837c61bb71d54267d
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2e7fdc07fa2a01198df28573e62b56b3e5f96022
                                          19f7733bb892ff6b682d26a57b027d1236883705)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} e43370c7ecc79c30de6e8ee0d1f43691c5f0bd03
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES b6f7a1da18f2a17bc32acd37b539cfe1f518e51a
                                          eedde2c7a36dee20eb2c0636fcdcb713e38c18e9)
    ENDIF()
ELSE()
    # Set platform specific configuration options
    # Note #1: we would normally rely on [QtWebKit]/Tools/Scripts/build-webkit
    #          to build and install QtWebKit, but this will not only build
    #          QtWebKit in a folder under [QtWebKit] (!!), but it will also (try
    #          to) install QtWebKit under [Qt] (!!) while we want to build
    #          QtWebKit out of source and to install it under this directory.
    #          So, instead, we build and install QtWebKit 'manually'...
    # Note #2: on Windows, we need to set some environment variables, something
    #          that cannot be done using ExternalProject_Add, hence we don't
    #          have a configure command as such and we call a batch file as part
    #          of our build command...

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    SET(QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR ${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD})

    SET(BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/${QTWEBKIT_IMPORT_LIBRARY}
        <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    )

    IF(WIN32)
        SET(CONFIGURE_COMMAND ECHO:)
        SET(BUILD_COMMAND ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/buildCommand.bat)
        SET(INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
            && ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/include <INSTALL_DIR>/include
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/bin/${QTWEBKIT_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKIT_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKIT_IMPORT_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITD_SHARED_LIBRARY} <INSTALL_DIR>/bin/${QTWEBKITD_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITD_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKITD_IMPORT_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/bin/${QTWEBKITWIDGETS_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETS_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETSD_SHARED_LIBRARY} <INSTALL_DIR>/bin/${QTWEBKITWIDGETSD_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETSD_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKITWIDGETSD_IMPORT_LIBRARY}
        )

        LIST(APPEND BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${QTWEBKITD_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${QTWEBKITWIDGETSD_IMPORT_LIBRARY}
        )
        
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/buildCommand.bat.in
                        ${BUILD_COMMAND})
    ELSE()
        SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} <SOURCE_DIR>)
        SET(BUILD_COMMAND ${MAKE_JOM_COMMAND})

        IF(APPLE)
            IF(RELEASE_MODE)
                SET(BUILD_COMMAND ${BUILD_COMMAND} release)
            ELSE()
                SET(BUILD_COMMAND ${BUILD_COMMAND} debug)
            ENDIF()

            SET(INSTALL_COMMAND
                   ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
                && ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKIT_SHARED_LIBRARY_DIR}/Headers <INSTALL_DIR>/lib/${QTWEBKIT_SHARED_LIBRARY_DIR}/Headers
                && ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETS_SHARED_LIBRARY_DIR}/Headers <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_SHARED_LIBRARY_DIR}/Headers
                && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKIT_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E create_symlink 5 <INSTALL_DIR>/lib/${QTWEBKIT_DIR}/Versions/Current
                && ${CMAKE_COMMAND} -E create_symlink Versions/Current/Headers <INSTALL_DIR>/lib/${QTWEBKIT_DIR}/Headers
                && ${CMAKE_COMMAND} -E create_symlink Versions/Current/QtWebKit <INSTALL_DIR>/lib/${QTWEBKIT_DIR}/QtWebKit
                && ${CMAKE_COMMAND} -E create_symlink 5 <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_DIR}/Versions/Current
                && ${CMAKE_COMMAND} -E create_symlink Versions/Current/Headers <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_DIR}/Headers
                && ${CMAKE_COMMAND} -E create_symlink Versions/Current/QtWebKitWidgets <INSTALL_DIR>/lib/${QTWEBKITWIDGETS_DIR}/QtWebKitWidgets
            )
        ELSE()
            SET(INSTALL_COMMAND
                   ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
                && ${CMAKE_COMMAND} -E copy_directory ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/include <INSTALL_DIR>/include
                && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${REAL_QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/lib/${REAL_QTWEBKIT_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E copy ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/lib/${REAL_QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/lib/${REAL_QTWEBKITWIDGETS_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKIT_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKIT_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKIT_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKIT_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKITWIDGETS_SHARED_LIBRARY}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKITWIDGETS_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}
                && ${CMAKE_COMMAND} -E create_symlink ${REAL_QTWEBKITWIDGETS_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_QTWEBKITWIDGETS_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}
            )
        ENDIF()
    ENDIF()

    # Build QtWebKit as an external project

    SET(SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${SOURCE_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qtwebkit
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${CONFIGURE_COMMAND}
        BUILD_COMMAND
            ${BUILD_COMMAND}
        INSTALL_COMMAND
            ${INSTALL_COMMAND}
        BUILD_BYPRODUCTS
            ${BUILD_BYPRODUCTS}
    )

    # Try to build our patch program

    SET(PATCH ${CMAKE_BINARY_DIR}/patchQtWebKitCMakeFile${CMAKE_EXECUTABLE_SUFFIX})
    SET(PATCH_SOURCE ${PROJECT_SOURCE_DIR}/cmake/patch.c)

    IF(WIN32)
        EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} ${PATCH_SOURCE} /link /out:${PATCH}
                        RESULT_VARIABLE RESULT)
    ELSE()
        EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -o ${PATCH} ${PATCH_SOURCE}
                        RESULT_VARIABLE RESULT)
    ENDIF()

    IF(NOT RESULT EQUAL 0)
        MESSAGE(FATAL_ERROR "patch could not be built...")
    ENDIF()

    # Patch our CMake files

    ExternalProject_Add_Step(${PACKAGE_BUILD} PatchCMakeFiles
                             COMMAND ${PATCH} Qt5WebKit/Qt5WebKitConfig.cmake
                             COMMAND ${PATCH} Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake
                             WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/cmake
                             DEPENDEES install)

    # Make sure that we have the actual contents of our header files
    # Note: this is only for Windows and Linux since, on those platforms, some
    #       of our 'installed' header files are indeed #including our copy of
    #       the QtWebKit header files rather than having their actual
    #       contents...

    IF(WIN32 OR NOT APPLE)
        ExternalProject_Add_Step(${PACKAGE_BUILD} PatchHeaderFiles
                                 COMMAND ${CMAKE_COMMAND} -D SOURCE_DIR=${SOURCE_DIR}
                                                          -D INCLUDE_DIR=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include
                                                          -P ${PROJECT_SOURCE_DIR}/cmake/patch.cmake
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${REAL_LOCAL_EXTERNAL_PACKAGE_DIR}
                        TARGET_PLATFORM ${TARGET_PLATFORM}
                        PACKAGED_FILES .
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Make sure that QtWebKit and its CMake files can be found

SET(QTWEBKIT_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)

SET(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${QTWEBKIT_DIR}/ext/cmake/Qt5WebKit
    ${QTWEBKIT_DIR}/ext/cmake/Qt5WebKitWidgets
    PARENT_SCOPE
)

SET(QTWEBKIT_DIR ${QTWEBKIT_DIR} PARENT_SCOPE)
