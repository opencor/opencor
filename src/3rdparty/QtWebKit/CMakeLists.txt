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

# Release tag

SET(RELEASE_TAG v5.212.0-alpha2)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    IF(RELEASE_MODE)
        SET(DEBUG_TAG)
    ELSE()
        SET(DEBUG_TAG d)
    ENDIF()

    SET(QTWEBKIT_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKIT_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(QTWEBKITWIDGETS_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QTWEBKITWIDGETS_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(ICUDT_SHARED_LIBRARY icudt${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ICUIN_SHARED_LIBRARY icuin${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ICUUC_SHARED_LIBRARY icuuc${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(JPEG_FILENAME libjpeg.9.dylib)
    SET(PNG_FILENAME libpng16.16.dylib)

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
    SET(IMPORT_LIBRARIES_DIR lib)
ELSE()
    IF(APPLE)
        SET(SHARED_LIBRARIES_DIR lib)
    ELSE()
        SET(SHARED_LIBRARIES_DIR lib/x86_64-linux-gnu)
    ENDIF()

    SET(IMPORT_LIBRARIES_DIR ${SHARED_LIBRARIES_DIR})
ENDIF()

SET(SHA1_FILES
    ${SHARED_LIBRARIES_DIR}/${QTWEBKIT_SHARED_LIBRARY}
    ${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        ${SHARED_LIBRARIES_DIR}/${ICUDT_SHARED_LIBRARY}
        ${SHARED_LIBRARIES_DIR}/${ICUIN_SHARED_LIBRARY}
        ${SHARED_LIBRARIES_DIR}/${ICUUC_SHARED_LIBRARY}

        ${IMPORT_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
        ${IMPORT_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    )
ELSEIF(APPLE)
    LIST(APPEND SHA1_FILES
        ${SHARED_LIBRARIES_DIR}/${JPEG_FILENAME}
        ${SHARED_LIBRARIES_DIR}/${PNG_FILENAME}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 4e12a551320fbc9ab6b967cb96f7e4451fd8efc2
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 28d1cdb58863f084355864e7ce99646bc59db6fc
                                              84f7a27c2dfa03c239b07533262387f2473b2bd1
                                              874175a6db51afe0902882f0b00f7855ab498c79
                                              47cb72ba7c899ca218be75dd89a0c0f900b968b8
                                              a386658900a742f31f1f08d55c0915d577af2d7e
                                              76bf04efaa836ef76eed2b6ed22c9b2e29bb31ad
                                              44f6e6f1106ebb1ebb3029ae0636d377ff254b8f)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 7270bbc47a668e383def1513a23cde7bc341b1e9
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES aa46f4c753aab3b514802a1404ca1d98c0260795
                                              1c4779d99c6379a1361d22718a65b6f74e0f5ca2
                                              43497bf1a89173badccc0b945b43473cf5d5f12a
                                              2ef114df767c6fe4184ef458545ff8d71f4f4917
                                              5eeaf7c066cca05413a35bba5efc71526c46f478
                                              13e16b8509579a170eab540d93050adde0d2f760
                                              995567217f17abd65322dd4dcb108dedc89c1bfa)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 6e1c99acef94f66adcff77f6f68d51c3c37b38e0
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 869f0640969325793ee1620d4c941f20050b27bd
                                          fb40bf07a6cde27fc57122b5c47a02a8023ffd60
                                          b45f5b149930f86ee6009e06569e4b339d45808a
                                          3d2fd6309859eb5ba306b9c6a5be578784562a48)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} d37d37c8d537add58870b967795968bae57ddea4
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES a045297a97db11c3940d29b04dbc6db29931d82b
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
            https://github.com/opencor/qtwebkit
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
            -DFULL_LOCAL_EXTERNAL_PACKAGE_DIR=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            -DGIT_TAG=${GIT_TAG}
            -DICUDT_SHARED_LIBRARY=${ICUDT_SHARED_LIBRARY}
            -DICUIN_SHARED_LIBRARY=${ICUIN_SHARED_LIBRARY}
            -DICUUC_SHARED_LIBRARY=${ICUUC_SHARED_LIBRARY}
            -DPORT=Qt
            -DUSE_GSTREAMER=OFF
            -DUSE_LIBHYPHEN=OFF
            -DUSE_MEDIA_FOUNDATION=OFF
            -DUSE_QT_MULTIMEDIA=ON
            -DUSE_SYSTEM_MALLOC=OFF
            -DUSE_WOFF2=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
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

    IF(NOT WIN AND NOT APPLE)
        SET(SYMBOLIC_LINKS
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}

            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}
        )
    ENDIF()

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES
                            include
                            ${IMPORT_LIBRARIES_DIR}/cmake
                            ${SHA1_FILES}
                            ${SYMBOLIC_LINKS}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our QtWebKit package
# Note: on Windows, we distinguish between release and debug modes, so remove
#       that piece of information...

IF(WIN32)
    STRING(REGEX REPLACE "/[^/]+$" "" REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
ELSE()
    SET(REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
ENDIF()

SET(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${IMPORT_LIBRARIES_DIR}/cmake
    PARENT_SCOPE
)

SET(QTWEBKIT_ROOT_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(QTWEBKIT_INCLUDE_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(QTWEBKIT_BINARIES_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin PARENT_SCOPE)
SET(QTWEBKIT_LIBRARIES_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR} PARENT_SCOPE)
SET(QTWEBKIT_QTWEBKIT_LIBRARY ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QTWEBKIT_QTWEBKITWIDGETS_LIBRARY ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QTWEBKIT_LIBRARIES
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    PARENT_SCOPE
)

IF(APPLE)
    SET(QTWEBKIT_JPEG_LIBRARY ${JPEG_FILENAME} PARENT_SCOPE)
    SET(QTWEBKIT_PNG_LIBRARY ${PNG_FILENAME} PARENT_SCOPE)
ENDIF()
