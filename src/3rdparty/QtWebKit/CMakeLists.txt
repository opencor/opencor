PROJECT(QtWebKit)

# Name and version of our package

SET(PACKAGE_NAME QtWebKit)
SET(PACKAGE_VERSION 5.9.2)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 5)
SET(MINOR_LIBRARY_VERSION 9)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.2)

# Git tag for our library

SET(GIT_TAG v5.9.2)

# Platform target for our library

SET(TARGET_PLATFORM ${PLATFORM})

# Specify where our local package will be installed
# Note: on Windows, we would normally distinguish between release and debug
#       modes, but QtWebKit's build system doesn't, hence we go one folder up...

IF(WIN32)
    STRING(REGEX REPLACE "/[^/]+$" "" REAL_LOCAL_EXTERNAL_PACKAGE_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR})
ELSE()
    SET(REAL_LOCAL_EXTERNAL_PACKAGE_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR})
ENDIF()

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

    SET(ICUDT_SHARED_LIBRARY icudt${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ICUIN_SHARED_LIBRARY icuin${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ICUUC_SHARED_LIBRARY icuuc${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
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

        bin/${ICUDT_SHARED_LIBRARY}
        bin/${ICUIN_SHARED_LIBRARY}
        bin/${ICUUC_SHARED_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    IF(WIN32)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 8dbe5333fd85bb3d02b3ee34ebcf116bf5005792
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES dd553394bc30b2abd837684a0cc8fcd8e8290735
                                          6cd9bc498817366f0158b54ec1df4501e5a84edb
                                          eb12d7fecced18da57590421265c5dbb79a314d9
                                          87d68e4885b35de0f3118068f16fd2960cc9e667
                                          f6db0a70c3f5405fa6526587de775562327ed83a
                                          773ea29754a4d252267555c4bbaca7cfe2bcf569
                                          dfd1812b3e43af731fca6bbb02c6f1a0b38639f1
                                          3aa536cca6c1ce5e8ce0d1bc6a8be34ddd64ae5a
                                          874175a6db51afe0902882f0b00f7855ab498c79
                                          47cb72ba7c899ca218be75dd89a0c0f900b968b8
                                          a386658900a742f31f1f08d55c0915d577af2d7e)
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} baa9208a7bdce049a4b689ee0119701c866b7f46
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2947b118b4328463378928fe3a0a0ec6aa7cc5db
                                          ffb1456fd920fc4525016a2a11739cffa4cae769)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 6334220e0054dbb0303146808cdca35834007780
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f4177885615db27f8d34439dae7a88a9faa8d13c
                                          4f7eeeecca9323bae310f2bcb4a078e217723279)
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
            && ${CMAKE_COMMAND} -E copy C:/icu/bin64/${ICUDT_SHARED_LIBRARY} <INSTALL_DIR>/bin/${ICUDT_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy C:/icu/bin64/${ICUIN_SHARED_LIBRARY} <INSTALL_DIR>/bin/${ICUIN_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy C:/icu/bin64/${ICUUC_SHARED_LIBRARY} <INSTALL_DIR>/bin/${ICUUC_SHARED_LIBRARY}
        )

        LIST(APPEND BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${QTWEBKITD_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${QTWEBKITWIDGETSD_IMPORT_LIBRARY}
        )

        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/buildCommand.bat.in
                       ${BUILD_COMMAND})
    ELSEIF(APPLE)
        SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} <SOURCE_DIR>)

        IF(RELEASE_MODE)
            SET(BUILD_COMMAND ${MAKE_JOM_COMMAND} release)
        ELSE()
            SET(BUILD_COMMAND ${MAKE_JOM_COMMAND} debug)
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
        # Build ICU as an external package

        SET(ICU_INSTALL_DIR ${EXTERNAL_PROJECT_BUILD_DIR}/Build/icuBuild/install)

        ExternalProject_Add(icuBuild
            SOURCE_DIR
                ${CMAKE_SOURCE_DIR}/ext/icu
            INSTALL_DIR
                ${ICU_INSTALL_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/icu
            GIT_TAG
                v56.1
            CONFIGURE_COMMAND
                <SOURCE_DIR>/source/runConfigureICU Linux --prefix=<INSTALL_DIR>
            BUILD_COMMAND
                ${MAKE_JOM_COMMAND}
            INSTALL_COMMAND
                ${MAKE_JOM_COMMAND} install
        )

        SET(CONFIGURE_COMMAND echo)
        SET(BUILD_COMMAND ${QTWEBKIT_EXTERNAL_PROJECT_BUILD_DIR}/buildCommand)
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

        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/buildCommand.in
                       ${BUILD_COMMAND})
    ENDIF()

    # Build QtWebKit as an external project

    SET(SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    IF(NOT WIN32 AND NOT APPLE)
        SET(DEFINE_DEPENDS
            DEPENDS
                icuBuild
        )
    ELSE()
        SET(DEFINE_DEPENDS)
    ENDIF()

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
        ${DEFINE_DEPENDS}
    )

    # (Try to) build our patch CMake program

	BUILD_PATCHCMAKE()

    # Patch our CMake files

    ExternalProject_Add_Step(${PACKAGE_BUILD} PatchCMakeFiles
                             COMMAND ${PATCH} Qt5WebKit/Qt5WebKitConfig.cmake WebKit WebKit
                             COMMAND ${PATCH} Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake WebKitWidgets WebKit
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
                                                          -P ${CMAKE_SOURCE_DIR}/cmake/patchheaderfiles.cmake
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${REAL_LOCAL_EXTERNAL_PACKAGE_DIR}
                        TARGET_PLATFORM ${TARGET_PLATFORM}
                        PACKAGED_FILES .
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
ENDIF()

# Make sure that QtWebKit and its CMake files can be found

SET(QTWEBKIT_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)

SET(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${QTWEBKIT_DIR}/ext/cmake/Qt5WebKit
    ${QTWEBKIT_DIR}/ext/cmake/Qt5WebKitWidgets
    PARENT_SCOPE
)

SET(QTWEBKIT_BINARIES_DIR ${QTWEBKIT_DIR}/ext/bin PARENT_SCOPE)
SET(QTWEBKIT_LIBRARIES_DIR ${QTWEBKIT_DIR}/ext/lib PARENT_SCOPE)
