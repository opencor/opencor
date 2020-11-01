project(QtWebKit)

# Name and version of our package

set(PACKAGE_NAME QtWebKit)
set(PACKAGE_VERSION 5.212.0)

# Version of our library

set(MAJOR_LIBRARY_VERSION 5)
set(MINOR_LIBRARY_VERSION 212)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tags for our libraries

set(GIT_TAG 0f85ce57ba143616099efb5337a56eb4450f5f4f)
set(GIT_TAG_LIBJPEG 1.5.90)
set(GIT_TAG_LIBPNG v1.6.34)
set(GIT_TAG_ZLIB v1.2.11)

# Release tag

set(RELEASE_TAG v5.12.0)

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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 88c2375d8f90c3c6e731a52002bcb4695163c489
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 6eeb3b514038fba691bdcf94183c35bc04b1029a
                                              154194a3dfe1723716da77a2938b0757ec1bb326
                                              874175a6db51afe0902882f0b00f7855ab498c79
                                              47cb72ba7c899ca218be75dd89a0c0f900b968b8
                                              a386658900a742f31f1f08d55c0915d577af2d7e
                                              9ec4fe53b4189db4d2b3791a4f994384ab910641
                                              0753c0147fc75d617612088e745f17b3107d1ada)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 56b08867163478535787e0014b65c93b4487873c
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES f89689041007123e2f87f425c153ad236d490f2c
                                              6bfec618a49e104b1dde9547f48b5ded382ec39b
                                              43497bf1a89173badccc0b945b43473cf5d5f12a
                                              2ef114df767c6fe4184ef458545ff8d71f4f4917
                                              5eeaf7c066cca05413a35bba5efc71526c46f478
                                              c1f07535c93d5d51cc211813f48d0ca827cda1f5
                                              a89cf300bea576d849ee66a2b33832a65c0db9ff)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 907aa8f0572de663a1d10386c71a79afb7acbba6
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 024104337ec2a92f2ae63bdd592cb9ac7c426b35
                                          cbace54abac7c6df21d1c0f8a2e00fc9a0c9a696)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} bba1e82441095e952716b247ce280dd51721bb22
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 687d5b9d508ed450315cc8b93dfb269696921ebe
                                          91fd09d27e4e80cf6ac6e60a6263abb09e690989)
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
