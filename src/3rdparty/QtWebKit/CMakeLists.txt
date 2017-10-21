PROJECT(QtWebKit)

# Name and version of our package

SET(PACKAGE_NAME QtWebKit)
SET(PACKAGE_VERSION 5.212.0)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 5)
SET(MINOR_LIBRARY_VERSION 212)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tag for our library

SET(GIT_TAG v5.212.0-alpha2)

# Package repository and release tag

SET(PACKAGE_REPOSITORY qtwebkitreloaded)
SET(RELEASE_TAG v5.212.0-alpha2)

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
    SET(QTWEBKIT_SHARED_LIBRARY_DIR QtWebKit.framework/Versions/${QT_VERSION_MAJOR})
    SET(QTWEBKIT_SHARED_LIBRARY ${QTWEBKIT_SHARED_LIBRARY_DIR}/QtWebKit)
    SET(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    SET(QTWEBKITWIDGETS_SHARED_LIBRARY QtWebKitWidgets.framework/Versions/${QT_VERSION_MAJOR}/QtWebKitWidgets)
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
ELSE()
    SET(QTWEBKIT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    SET(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    SET(QTWEBKITWIDGETS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
ENDIF()

# Specify the files that need to have their SHA-1 value checked

IF(WIN32)
    SET(SHARED_LIBRARIES_DIR bin)
ELSEIF(APPLE)
    SET(SHARED_LIBRARIES_DIR lib)
ELSE()
    SET(SHARED_LIBRARIES_DIR lib/x86_64-linux-gnu)
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

# Libraries, if any, that also need to be deployed along with our QtWebKit
# package

IF(APPLE)
    SET(JPEG_FILENAME libjpeg.9.dylib)
    SET(PNG_FILENAME libpng16.16.dylib)
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    IF(WIN32)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 8dbe5333fd85bb3d02b3ee34ebcf116bf5005792
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
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
                              ${RELATIVE_PROJECT_SOURCE_DIR} b289e92b3e121e834d03ed712bdbd6dc7b593e82
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES a5502fa1b4da8e20daea6f3459a82565377adb39
                                          fb40bf07a6cde27fc57122b5c47a02a8023ffd60)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f09bf98f84971b84e980d751a05a408efa7eaf47
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f274a45c84639284601cb838b0bf4b7a84e47ca4
                                          1ca7ba8178d833a09f78ef0df2e452ab43b0b9d7)
    ENDIF()
ELSE()
    # Build QtWebKit as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qtwebkitreloaded
        GIT_TAG
            ${GIT_TAG}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DENABLE_ACCELERATED_2D_CANVAS=ON
            -DENABLE_ALLINONE_BUILD=ON
            -DENABLE_API_TESTS=ON
            -DENABLE_CSS_GRID_LAYOUT=ON
            -DENABLE_DATABASE_PROCESS=ON
            -DENABLE_DATALIST_ELEMENT=ON
            -DENABLE_DEVICE_ORIENTATION=ON
            -DENABLE_DRAG_SUPPORT=ON
            -DENABLE_FULLSCREEN_API=ON
            -DENABLE_GAMEPAD_DEPRECATED=OFF
            -DENABLE_GEOLOCATION=ON
            -DENABLE_ICONDATABASE=ON
            -DENABLE_INDEXED_DATABASE=ON
            -DENABLE_INSPECTOR_UI=ON
            -DENABLE_JIT=ON
            -DENABLE_LEGACY_WEB_AUDIO=OFF
            -DENABLE_LINK_PREFETCH=ON
            -DENABLE_MEDIA_SOURCE=OFF
            -DENABLE_NETSCAPE_PLUGIN_API=OFF
            -DENABLE_OPENGL=ON
            -DENABLE_PRINT_SUPPORT=ON
            -DENABLE_QT_GESTURE_EVENTS=ON
            -DENABLE_QT_WEBCHANNEL=ON
            -DENABLE_SAMPLING_PROFILER=ON
            -DENABLE_SPELLCHECK=ON
            -DENABLE_TOUCH_EVENTS=ON
            -DENABLE_VIDEO=ON
            -DENABLE_WEBKIT2=ON
            -DENABLE_WEB_AUDIO=OFF
            -DENABLE_X11_TARGET=OFF
            -DENABLE_XSLT=ON
            -DPORT=Qt
            -DUSE_GSTREAMER=OFF
            -DUSE_LIBHYPHEN=OFF
            -DUSE_MEDIA_FOUNDATION=OFF
            -DUSE_QT_MULTIMEDIA=ON
            -DUSE_SYSTEM_MALLOC=OFF
            -DUSE_WOFF2=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
            <INSTALL_DIR>/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(APPLE)
        # Copy over some libraries needed by QtWebKit package

        FIND_PACKAGE(JPEG REQUIRED)
        FIND_PACKAGE(PNG REQUIRED)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibraries
                                 COMMAND ${CMAKE_COMMAND} -E copy ${JPEG_LIBRARY} ${JPEG_FILENAME}
                                 COMMAND chmod 755 ${JPEG_FILENAME}
                                 COMMAND strip -x ${JPEG_FILENAME}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${PNG_LIBRARY} ${PNG_FILENAME}
                                 COMMAND chmod 755 ${PNG_FILENAME}
                                 COMMAND strip -x ${PNG_FILENAME}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}
                                 DEPENDEES install)

        # Fix the RPATH information

        ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                 COMMAND install_name_tool -change /usr/local/opt/jpeg/lib/${JPEG_FILENAME}
                                                                   @rpath/${JPEG_FILENAME}
                                                                   ${QTWEBKIT_SHARED_LIBRARY}
                                 COMMAND install_name_tool -change /usr/local/opt/libpng/lib/${PNG_FILENAME}
                                                                   @rpath/${PNG_FILENAME}
                                                                   ${QTWEBKIT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    IF(APPLE)
        SET(JPEG_FILE ${SHARED_LIBRARIES_DIR}/${JPEG_FILENAME})
        SET(PNG_FILE ${SHARED_LIBRARIES_DIR}/${PNG_FILENAME})
    ELSE()
        SET(JPEG_FILE)
        SET(PNG_FILE)
    ENDIF()

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES
                            include
                            ${SHARED_LIBRARIES_DIR}/cmake
                            ${JPEG_FILE}
                            ${PNG_FILE}
                            ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our QtWebKit package

SET(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/cmake
    PARENT_SCOPE
)

SET(QTWEBKIT_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(QTWEBKIT_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(QTWEBKIT_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin PARENT_SCOPE)
SET(QTWEBKIT_LIBRARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR} PARENT_SCOPE)
SET(QTWEBKIT_QTWEBKIT_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QTWEBKIT_QTWEBKITWIDGETS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QTWEBKIT_LIBRARIES
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    PARENT_SCOPE
)

IF(APPLE)
    SET(QTWEBKIT_JPEG_LIBRARY ${JPEG_FILENAME} PARENT_SCOPE)
    SET(QTWEBKIT_PNG_LIBRARY ${PNG_FILENAME} PARENT_SCOPE)
ENDIF()
