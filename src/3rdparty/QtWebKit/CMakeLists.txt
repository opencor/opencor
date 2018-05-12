project(QtWebKit)

# Name and version of our package

set(PACKAGE_NAME QtWebKit)
set(PACKAGE_VERSION 5.212.0)

# Version of our library

set(MAJOR_LIBRARY_VERSION 5)
set(MINOR_LIBRARY_VERSION 212)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tags for our libraries

set(GIT_TAG v5.212.0-alpha2-72cfbd7-extra)
set(GIT_TAG_LIBJPEG 1.5.90)
set(GIT_TAG_LIBPNG v1.6.34)
set(GIT_TAG_ZLIB v1.2.11)

# Release tag

set(RELEASE_TAG v5.212.0-alpha2-72cfbd7-extra)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    if(RELEASE_MODE)
        set(DEBUG_TAG)
    else()
        set(DEBUG_TAG d)
    endif()

    set(QTWEBKIT_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QTWEBKIT_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKit${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(QTWEBKITWIDGETS_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QTWEBKITWIDGETS_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}WebKitWidgets${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(ICUDT_SHARED_LIBRARY icudt${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(ICUIN_SHARED_LIBRARY icuin${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(ICUUC_SHARED_LIBRARY icuuc${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
elseif(APPLE)
    set(QTWEBKIT_SHARED_LIBRARY QtWebKit.framework/Versions/${QT_VERSION_MAJOR}/QtWebKit)
    set(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    set(QTWEBKITWIDGETS_SHARED_LIBRARY QtWebKitWidgets.framework/Versions/${QT_VERSION_MAJOR}/QtWebKitWidgets)
    set(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
else()
    set(QTWEBKIT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    set(QTWEBKIT_IMPORT_LIBRARY ${QTWEBKIT_SHARED_LIBRARY})

    set(QTWEBKITWIDGETS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    set(QTWEBKITWIDGETS_IMPORT_LIBRARY ${QTWEBKITWIDGETS_SHARED_LIBRARY})
endif()

# Specify the files that need to have their SHA-1 value checked

if(WIN32)
    set(SHARED_LIBRARIES_DIR bin)
    set(IMPORT_LIBRARIES_DIR lib)
else()
    if(APPLE)
        set(SHARED_LIBRARIES_DIR lib)
    else()
        set(SHARED_LIBRARIES_DIR lib/x86_64-linux-gnu)
    endif()

    set(IMPORT_LIBRARIES_DIR ${SHARED_LIBRARIES_DIR})
endif()

set(SHA1_FILES
    ${SHARED_LIBRARIES_DIR}/${QTWEBKIT_SHARED_LIBRARY}
    ${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        ${SHARED_LIBRARIES_DIR}/${ICUDT_SHARED_LIBRARY}
        ${SHARED_LIBRARIES_DIR}/${ICUIN_SHARED_LIBRARY}
        ${SHARED_LIBRARIES_DIR}/${ICUUC_SHARED_LIBRARY}

        ${IMPORT_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
        ${IMPORT_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e23924945aef287186ef81c5c52b3789283e7cf8
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 00a473546911c3790c1b2e52e2f852031c481a67
                                              8db0a39e62584ce35748606ddf0671e8d675b323
                                              874175a6db51afe0902882f0b00f7855ab498c79
                                              47cb72ba7c899ca218be75dd89a0c0f900b968b8
                                              a386658900a742f31f1f08d55c0915d577af2d7e
                                              2fa2e092ac53a7962a28e304c6fb3bdfd8bf02b7
                                              16089ad4d6d4f8ad5b0d8a350c38eaab1073b1a4)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 8f7d0ebd288de615842c46398fdd15062a8898e4
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9bedec01f53ff0e60ad18fe014c301141f0d596b
                                              2483d5d995ce8b5cccf91be2a40c90baf9f07cdf
                                              43497bf1a89173badccc0b945b43473cf5d5f12a
                                              2ef114df767c6fe4184ef458545ff8d71f4f4917
                                              5eeaf7c066cca05413a35bba5efc71526c46f478
                                              504f6aa0ff6a6db90bf94fb2482ffdf61fb30600
                                              81a80ab272e675442e44b3c2f8ecaf7e237027f1)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} da18544d152ff1e012fc65419b4e894a6283558f
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 32502b8e69303ae0b74baba50318bc7ae6f3b227
                                          17b3339432e9f99f9b702d4d85c04225c4d3c9d1
                                          8db6c231add451213cd7cc74423b19cfc4916689
                                          3d2fd6309859eb5ba306b9c6a5be578784562a48)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 96988d34068d3343852bb27417dae05c15934793
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 1f55a8c4de16252e539889200e7022e23c565e14
                                          48c5de3d1723388885aea0771a63730dc30a9040)
    endif()
else()
    # Build JPEG, PNG and ZLIB as external projects, if on Linux or macOS

    if(NOT WIN32)
        set(DEPENDS libjpegBuild libpngBuild zlibBuild)

        # JPEG

        set(LIBJPEG_STATIC_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/libturbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX})

        ExternalProject_Add(libjpegBuild
            SOURCE_DIR
                ${CMAKE_SOURCE_DIR}/ext/libjpeg-turbo
            INSTALL_DIR
                ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/libjpeg-turbo
            GIT_TAG
                ${GIT_TAG_LIBJPEG}
            CMAKE_GENERATOR
                ${CMAKE_GENERATOR}
            CMAKE_ARGS
                ${CMAKE_ARGS}
                -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DENABLE_SHARED=OFF
            BUILD_BYPRODUCTS
                ${LIBJPEG_STATIC_LIBRARY}
        )

        # PNG

        set(LIBPNG_STATIC_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/libpng16${CMAKE_STATIC_LIBRARY_SUFFIX})

        ExternalProject_Add(libpngBuild
            SOURCE_DIR
                ${CMAKE_SOURCE_DIR}/ext/libpng
            INSTALL_DIR
                ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/libpng
            GIT_TAG
                ${GIT_TAG_LIBPNG}
            CMAKE_GENERATOR
                ${CMAKE_GENERATOR}
            CMAKE_ARGS
                ${CMAKE_ARGS}
                -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DPNG_SHARED=OFF
                -DPNG_TESTS=OFF
            BUILD_BYPRODUCTS
                ${LIBPNG_STATIC_LIBRARY}
        )

        # ZLIB

        set(ZLIB_STATIC_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/libz${CMAKE_STATIC_LIBRARY_SUFFIX})

        ExternalProject_Add(zlibBuild
            SOURCE_DIR
                ${CMAKE_SOURCE_DIR}/ext/zlib
            INSTALL_DIR
                ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/zlib
            GIT_TAG
                ${GIT_TAG_ZLIB}
            CMAKE_GENERATOR
                ${CMAKE_GENERATOR}
            CMAKE_ARGS
                ${CMAKE_ARGS}
                -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            BUILD_BYPRODUCTS
                ${ZLIB_STATIC_LIBRARY}
        )
    endif()

    # Build QtWebKit as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    if(WIN32)
        set(CMAKE_EXTRA_ARGS
            -DFULL_LOCAL_EXTERNAL_PACKAGE_DIR=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            -DGIT_TAG=${GIT_TAG}
            -DICUDT_SHARED_LIBRARY=${ICUDT_SHARED_LIBRARY}
            -DICUIN_SHARED_LIBRARY=${ICUIN_SHARED_LIBRARY}
            -DICUUC_SHARED_LIBRARY=${ICUUC_SHARED_LIBRARY}
        )
    else()
        set(CMAKE_EXTRA_ARGS
            -DQT_BUNDLED_JPEG=ON
            -DJPEG_INCLUDE_DIR=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include
            -DJPEG_LIBRARIES=${LIBJPEG_STATIC_LIBRARY}
            -DQT_BUNDLED_PNG=ON
            -DPNG_INCLUDE_DIRS=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include
            -DPNG_LIBRARIES=${LIBPNG_STATIC_LIBRARY}
            -DQT_BUNDLED_ZLIB=ON
            -DZLIB_INCLUDE_DIRS=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include
            -DZLIB_LIBRARIES=${ZLIB_STATIC_LIBRARY}
        )

        if(NOT APPLE)
            list(APPEND CMAKE_EXTRA_ARGS
                -DICU_ROOT_DIR=${ICU_ROOT_DIR}
                -DICU_ICUI18N_LIBRARY=${ICU_ICUI18N_LIBRARY}
                -DICU_ICUUC_LIBRARY=${ICU_ICUUC_LIBRARY}
            )
        endif()
    endif()

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
            ${CMAKE_EXTRA_ARGS}
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
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
        DEPENDS
            ${DEPENDS}
    )

    # Package our external project

    if(NOT WIN AND NOT APPLE)
        set(SYMBOLIC_LINKS
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}

            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}
        )
    endif()

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES
                            include
                            ${IMPORT_LIBRARIES_DIR}/cmake
                            ${SHA1_FILES}
                            ${SYMBOLIC_LINKS}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our QtWebKit package
# Note: on Windows, we distinguish between release and debug modes, so remove
#       that piece of information...

if(WIN32)
    string(REGEX REPLACE "/[^/]+$" "" REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
else()
    set(REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
endif()

set(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${IMPORT_LIBRARIES_DIR}/cmake
    PARENT_SCOPE
)

set(QTWEBKIT_ROOT_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(QTWEBKIT_INCLUDE_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(QTWEBKIT_BINARIES_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin PARENT_SCOPE)
set(QTWEBKIT_LIBRARIES_DIR ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR} PARENT_SCOPE)
set(QTWEBKIT_QTWEBKIT_LIBRARY ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY} PARENT_SCOPE)
set(QTWEBKIT_QTWEBKITWIDGETS_LIBRARY ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY} PARENT_SCOPE)
set(QTWEBKIT_LIBRARIES
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
    ${REAL_FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    PARENT_SCOPE
)
