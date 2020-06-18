project(QtWebKit)

# Name and version of our package

set(PACKAGE_NAME QtWebKit)
set(PACKAGE_VERSION 5.212.0)

# Version of our library

set(MAJOR_LIBRARY_VERSION 5)
set(MINOR_LIBRARY_VERSION 212)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tags for our libraries

set(GIT_TAG v5.212.0-alpha3-opencor)
set(GIT_TAG_LIBJPEG 1.5.90)
set(GIT_TAG_LIBPNG v1.6.34)
set(GIT_TAG_ZLIB v1.2.11)

# Release tag

set(RELEASE_TAG v5.212.0-alpha3-opencor)

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

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 4f2b929bc80a3d23ecb2e1e558cd358542242444
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 49094bb6f4060ee51ef3add6e46fbe4b138dc252
                                              a61f75c32f9ec0802d79b437bca0a062e0e2bc07
                                              874175a6db51afe0902882f0b00f7855ab498c79
                                              47cb72ba7c899ca218be75dd89a0c0f900b968b8
                                              a386658900a742f31f1f08d55c0915d577af2d7e
                                              a7bde32bfba64fb037927b6e0355516cb4508ccc
                                              172fba699a361cc93597c4968cacd2cf44985095)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 1ccbb8d582ee72789dcbc4e39c6988380baf1022
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 8ddccd77d7dda04197233e8e3ec98e1ae0e884f8
                                              2c07c561b44dbb7db1c046dcc849db3558ec1f22
                                              43497bf1a89173badccc0b945b43473cf5d5f12a
                                              2ef114df767c6fe4184ef458545ff8d71f4f4917
                                              5eeaf7c066cca05413a35bba5efc71526c46f478
                                              c4b431401965c2fb2f139ffd462fe0a52213a615
                                              7ddf6870922fc2eb7db94d3f037a5c9ba8dd450d)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} e4c32b340516792d4d16807b1cac28bd5181abbe
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 818ce4e16892876260559494c6ae56f16935fd38
                                          06e5a1a883d52481d636a7fa790c745738bf20aa)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 2a8472a8894842cf515cac601705e4e8a4e80970
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 360c9d2b13f651e3544fe994618178421fc04b8e
                                          b7ec82ddcbb42f13783ef1c0ce3c67e97a1f980f)
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
            -DPORT=Qt
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
            <INSTALL_DIR>/${IMPORT_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
        DEPENDS
            ${DEPENDS}
    )

    # Package our external project

    if(NOT WIN32 AND NOT APPLE)
        set(SYMBOLIC_LINKS
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKit${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}

            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${SHARED_LIBRARIES_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}WebKitWidgets${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}
        )
    endif()

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
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

set(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${IMPORT_LIBRARIES_DIR}/cmake
    PARENT_SCOPE
)

set(QTWEBKIT_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(QTWEBKIT_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(QTWEBKIT_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin PARENT_SCOPE)
set(QTWEBKIT_LIBRARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR} PARENT_SCOPE)
set(QTWEBKIT_QTWEBKIT_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY} PARENT_SCOPE)
set(QTWEBKIT_QTWEBKITWIDGETS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY} PARENT_SCOPE)
set(QTWEBKIT_LIBRARIES
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKIT_IMPORT_LIBRARY}
    ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SHARED_LIBRARIES_DIR}/${QTWEBKITWIDGETS_IMPORT_LIBRARY}
    PARENT_SCOPE
)
