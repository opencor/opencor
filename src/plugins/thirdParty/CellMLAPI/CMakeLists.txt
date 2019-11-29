project(CellMLAPIPlugin)

# Name and version of our package

set(PACKAGE_NAME CellMLAPI)
set(PACKAGE_VERSION 1.12)

# Version of our package's libraries

set(MAJOR_ANNOTOOLS_LIBRARY_VERSION 2)
set(ANNOTOOLS_LIBRARY_VERSION 1.13)

set(MAJOR_CCGS_LIBRARY_VERSION 3)
set(CCGS_LIBRARY_VERSION 1.13)

set(MAJOR_CELEDS_LIBRARY_VERSION 1)
set(CELEDS_LIBRARY_VERSION 1.13)

set(MAJOR_CELEDSEXPORTER_LIBRARY_VERSION 1)
set(CELEDSEXPORTER_LIBRARY_VERSION 1.13)

set(MAJOR_CELLML_LIBRARY_VERSION 2)
set(CELLML_LIBRARY_VERSION 1.13)

set(MAJOR_CEVAS_LIBRARY_VERSION 1)
set(CEVAS_LIBRARY_VERSION 1.13)

set(MAJOR_CUSES_LIBRARY_VERSION 1)
set(CUSES_LIBRARY_VERSION 1.13)

set(MAJOR_MALAES_LIBRARY_VERSION 1)
set(MALAES_LIBRARY_VERSION 1.13)

set(MAJOR_VACSS_LIBRARY_VERSION 1)
set(VACSS_LIBRARY_VERSION 1.13)

# Git tag for our library

set(GIT_TAG 1.12)

# Package repository and release tag

set(PACKAGE_REPOSITORY cellml-api)
set(RELEASE_TAG 1.12)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(ANNOTOOLS_SHARED_LIBRARY annotools${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(ANNOTOOLS_IMPORT_LIBRARY annotools${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CCGS_SHARED_LIBRARY ccgs${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CCGS_IMPORT_LIBRARY ccgs${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CELEDS_SHARED_LIBRARY celeds${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CELEDS_IMPORT_LIBRARY celeds${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CELEDSEXPORTER_SHARED_LIBRARY celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CELEDSEXPORTER_IMPORT_LIBRARY celedsexporter${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CELLML_SHARED_LIBRARY cellml${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CELLML_IMPORT_LIBRARY cellml${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CEVAS_SHARED_LIBRARY cevas${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CEVAS_IMPORT_LIBRARY cevas${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CUSES_SHARED_LIBRARY cuses${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CUSES_IMPORT_LIBRARY cuses${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(MALAES_SHARED_LIBRARY malaes${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(MALAES_IMPORT_LIBRARY malaes${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(VACSS_SHARED_LIBRARY vacss${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(VACSS_IMPORT_LIBRARY vacss${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(ANNOTOOLS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}annotools${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_ANNOTOOLS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(ANNOTOOLS_IMPORT_LIBRARY ${ANNOTOOLS_SHARED_LIBRARY})

    set(CCGS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CCGS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CCGS_IMPORT_LIBRARY ${CCGS_SHARED_LIBRARY})

    set(CELEDS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}celeds${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELEDS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CELEDS_IMPORT_LIBRARY ${CELEDS_SHARED_LIBRARY})

    set(CELEDSEXPORTER_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELEDSEXPORTER_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CELEDSEXPORTER_IMPORT_LIBRARY ${CELEDSEXPORTER_SHARED_LIBRARY})

    set(CELLML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cellml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELLML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CELLML_IMPORT_LIBRARY ${CELLML_SHARED_LIBRARY})

    set(CEVAS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cevas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CEVAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CEVAS_IMPORT_LIBRARY ${CEVAS_SHARED_LIBRARY})

    set(CUSES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cuses${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CUSES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CUSES_IMPORT_LIBRARY ${CUSES_SHARED_LIBRARY})

    set(MALAES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}malaes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_MALAES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(MALAES_IMPORT_LIBRARY ${MALAES_SHARED_LIBRARY})

    set(VACSS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}vacss${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_VACSS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(VACSS_IMPORT_LIBRARY ${VACSS_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${ANNOTOOLS_SHARED_LIBRARY}
    ${CCGS_SHARED_LIBRARY}
    ${CELEDS_SHARED_LIBRARY}
    ${CELEDSEXPORTER_SHARED_LIBRARY}
    ${CELLML_SHARED_LIBRARY}
    ${CEVAS_SHARED_LIBRARY}
    ${CUSES_SHARED_LIBRARY}
    ${MALAES_SHARED_LIBRARY}
    ${VACSS_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${ANNOTOOLS_SHARED_LIBRARY}
    lib/${CCGS_SHARED_LIBRARY}
    lib/${CELEDS_SHARED_LIBRARY}
    lib/${CELEDSEXPORTER_SHARED_LIBRARY}
    lib/${CELLML_SHARED_LIBRARY}
    lib/${CEVAS_SHARED_LIBRARY}
    lib/${CUSES_SHARED_LIBRARY}
    lib/${MALAES_SHARED_LIBRARY}
    lib/${VACSS_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${ANNOTOOLS_IMPORT_LIBRARY}
        lib/${CCGS_IMPORT_LIBRARY}
        lib/${CELEDS_IMPORT_LIBRARY}
        lib/${CELEDSEXPORTER_IMPORT_LIBRARY}
        lib/${CELLML_IMPORT_LIBRARY}
        lib/${CEVAS_IMPORT_LIBRARY}
        lib/${CUSES_IMPORT_LIBRARY}
        lib/${MALAES_IMPORT_LIBRARY}
        lib/${VACSS_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_CELLMLAPI_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} b8f7766f3ba7d3ac5b8f8c87bc9e8ae57dc9312c
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 1f1852678ea292832a79c8efc9f55d1cb8ea0ec8
                                              1b6aca37d31572a69dac43dd5789aeab5020d3b2
                                              231b40a80ea2404fd189b8a68a8d4760affcc801
                                              26e0398be1d1d935ad89b7601eeb7d649ef8b3be
                                              b8e97f1f740abc24349c4aff8b8f4041cd20eac4
                                              78715fa21806b1bfd9c6ee1014b8192733824e01
                                              5a0d21c0a0897b005c02221ae653075d5d11f4a5
                                              4d87163c2d6b75f06815eede8a185f10fc158980
                                              aa1fe801106f8408517021a1d959b4c0a80c3d49
                                              c6771b692f0b9b478700995a97e53773f01eef7c
                                              60ff6c2054053bd25bff9aa2e082ee594bbb16a6
                                              6a221eed1d076c8b03b7c2bc3fb47f806a45c440
                                              c1878b8c332ea3f1eac320c850c800e46c4e0ef3
                                              31dc601c4c0a928ddaa07c2b626bcd748f69c57e
                                              3547f3ca362aad907f96c25947cb4ab850377321
                                              88bdc009b40547fdb1469289758453b438084f1c
                                              d7eb189be16494898dbf0a3900198664eb41b578
                                              a0cf161095d238e2579d3e6d2190fd6c3bf662ef)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} ecfdeee15b981c0a044656210b09a66e32891647
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 39cda2ec74199626b13a32a09bafc31e790d997f
                                              a23745991a62ffedfdf95eb8f474f1488f4437a8
                                              6cf713242ed49b37fc5cf8afa884dbe228221789
                                              43ea731a55b50de58592ffc53bebddccadcbb0c5
                                              2017e69d36fec476471cc0c0f2d3d1fdf874e48d
                                              ec70b5368e5a8402d507560e4c54ef0ecc964396
                                              45dc4277bb416b2cee079bb8656e4f1a5a55aa94
                                              98555492f15bfcd60e01ba6976b1aeb93e0f55f6
                                              e43c49ccce3ada78770bf0d291a5b88a9f87e00c
                                              c6771b692f0b9b478700995a97e53773f01eef7c
                                              60ff6c2054053bd25bff9aa2e082ee594bbb16a6
                                              6a221eed1d076c8b03b7c2bc3fb47f806a45c440
                                              c1878b8c332ea3f1eac320c850c800e46c4e0ef3
                                              7dad0b59661d16fdf006592dcb0b531ea6f08963
                                              3547f3ca362aad907f96c25947cb4ab850377321
                                              88bdc009b40547fdb1469289758453b438084f1c
                                              d7eb189be16494898dbf0a3900198664eb41b578
                                              a0cf161095d238e2579d3e6d2190fd6c3bf662ef)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 0ae16141fe435ab7fbe9d9d4e024401bff7c91e2
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 12d0a63575bbe04cda516289bbc72493a49f479c
                                          27395a0a9990a2321ceec397b9fed8678885c227
                                          e21c57015e4c036c71d4f6df7d124e7a96be28fd
                                          9ca037e802d27932f69a8ffa835deb64949fdfa3
                                          9bf0abbdc923cfbebc5470f4ac7e5e012ee0cc9e
                                          e61db3c289a2e5c6a7793b9fffdc06c19c378a21
                                          6a523b4caaeb3a9e28bc2d244953e95d88f1ddd8
                                          701d6679ff656c439c4e669b8317c75561ec156c
                                          056062047102c43c5727752e1ddfd2382966661f)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 62f4f9e61fe1c9dfadbfbeb324f81eb618d1e104
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES bba7a197e467e2d0f456b689ba4ac61d0d691ba9
                                          a63ccaa2653827c48ae2f0dd09cb1cc68367c98d
                                          c374ec9d91e7a3ca58ada73d91302bb77c5b1f1c
                                          22b6e5ed021ae004712c3f8cd06f9f3a01b2d6b5
                                          d79e131d044122c70d284416fdcdb310bc3c748a
                                          2c883f78053574d23f3717d0349baa7a8f3f0d17
                                          6242794b4069e6072275455619244eeccd4c2c7a
                                          f078565570e72e36feea1b26258ccf3582c95cf4
                                          9ca52e05bb6772b8e01d650648c4c09045a21946)
    endif()
else()
    # Set some compilation flags and/or ignore some warnings

    if(WIN32)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4005 /wd4244 /wd4267 /wd4311 /wd4312")
        set(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4267 /wd4309 /wd4477 /wd4530 /wd4577")
    elseif(NOT APPLE)
        set(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated -Wno-misleading-indentation -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable")
    endif()

    # Build the CellML API as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${PACKAGE_CXX_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DBUILD_TESTING=OFF
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DENABLE_ANNOTOOLS=ON
            -DENABLE_CCGS=ON
            -DENABLE_CELEDS=ON
            -DENABLE_CELEDS_EXPORTER=ON
            -DENABLE_CEVAS=ON
            -DENABLE_CUSES=ON
            -DENABLE_DOCUMENTATION=OFF
            -DENABLE_MALAES=ON
            -DENABLE_RDF=ON
            -DENABLE_VACSS=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ANNOTOOLS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CCGS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CELEDS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CELEDSEXPORTER_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CELLML_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CEVAS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CUSES_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${MALAES_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${VACSS_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyAnnotoolsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${ANNOTOOLS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}annotools${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${ANNOTOOLS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${ANNOTOOLS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCcgsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CCGS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CCGS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CCGS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCeledsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELEDS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}celeds${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELEDS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELEDS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCeledsexporterLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELEDSEXPORTER_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELEDSEXPORTER_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELEDSEXPORTER_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCellmlLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELLML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cellml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELLML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELLML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCevasLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CEVAS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cevas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CEVAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CEVAS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCusesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CUSES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cuses${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CUSES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CUSES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyMalaesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${MALAES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}malaes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MALAES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${MALAES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyVacssLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${VACSS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}vacss${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${VACSS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${VACSS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our CellML API package

set(CELLMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(CELLMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(CELLMLAPI_ANNOTOOLS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${ANNOTOOLS_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CCGS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CCGS_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CELEDS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CELEDS_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CELEDSEXPORTER_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CELEDSEXPORTER_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CELLML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CELLML_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CEVAS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CEVAS_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_CUSES_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CUSES_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_MALAES_LIBRARY ${EXTERNAL_BINARIES_DIR}/${MALAES_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_VACSS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${VACSS_IMPORT_LIBRARY} PARENT_SCOPE)
set(CELLMLAPI_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_ANNOTOOLS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CCGS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CELEDS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CELEDSEXPORTER_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CELLML_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CEVAS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_CUSES_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_MALAES_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${CELLMLAPI_VACSS_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/cellmlapiplugin.cpp
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    TESTS
        tests
    DEPENDS_ON
        ${DEPENDS_ON}
)
