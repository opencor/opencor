PROJECT(CellMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME CellMLAPI)
SET(PACKAGE_VERSION 1.12)

# Version of our package's libraries

SET(MAJOR_ANNOTOOLS_LIBRARY_VERSION 2)
SET(ANNOTOOLS_LIBRARY_VERSION 1.13)

SET(MAJOR_CCGS_LIBRARY_VERSION 3)
SET(CCGS_LIBRARY_VERSION 1.13)

SET(MAJOR_CELEDS_LIBRARY_VERSION 1)
SET(CELEDS_LIBRARY_VERSION 1.13)

SET(MAJOR_CELEDSEXPORTER_LIBRARY_VERSION 1)
SET(CELEDSEXPORTER_LIBRARY_VERSION 1.13)

SET(MAJOR_CELLML_LIBRARY_VERSION 2)
SET(CELLML_LIBRARY_VERSION 1.13)

SET(MAJOR_CEVAS_LIBRARY_VERSION 1)
SET(CEVAS_LIBRARY_VERSION 1.13)

SET(MAJOR_CUSES_LIBRARY_VERSION 1)
SET(CUSES_LIBRARY_VERSION 1.13)

SET(MAJOR_MALAES_LIBRARY_VERSION 1)
SET(MALAES_LIBRARY_VERSION 1.13)

SET(MAJOR_VACSS_LIBRARY_VERSION 1)
SET(VACSS_LIBRARY_VERSION 1.13)

# Git tag for our library

SET(GIT_TAG 1.12)

# Package repository and release tag

SET(PACKAGE_REPOSITORY cellml-api)
SET(RELEASE_TAG 1.12)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(ANNOTOOLS_SHARED_LIBRARY annotools${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ANNOTOOLS_IMPORT_LIBRARY annotools${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CCGS_SHARED_LIBRARY ccgs${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CCGS_IMPORT_LIBRARY ccgs${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CELEDS_SHARED_LIBRARY celeds${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CELEDS_IMPORT_LIBRARY celeds${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CELEDSEXPORTER_SHARED_LIBRARY celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CELEDSEXPORTER_IMPORT_LIBRARY celedsexporter${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CELLML_SHARED_LIBRARY cellml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CELLML_IMPORT_LIBRARY cellml${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CEVAS_SHARED_LIBRARY cevas${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CEVAS_IMPORT_LIBRARY cevas${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CUSES_SHARED_LIBRARY cuses${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CUSES_IMPORT_LIBRARY cuses${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(MALAES_SHARED_LIBRARY malaes${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(MALAES_IMPORT_LIBRARY malaes${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(VACSS_SHARED_LIBRARY vacss${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(VACSS_IMPORT_LIBRARY vacss${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(ANNOTOOLS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}annotools${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_ANNOTOOLS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(ANNOTOOLS_IMPORT_LIBRARY ${ANNOTOOLS_SHARED_LIBRARY})

    SET(CCGS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CCGS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CCGS_IMPORT_LIBRARY ${CCGS_SHARED_LIBRARY})

    SET(CELEDS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}celeds${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELEDS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CELEDS_IMPORT_LIBRARY ${CELEDS_SHARED_LIBRARY})

    SET(CELEDSEXPORTER_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELEDSEXPORTER_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CELEDSEXPORTER_IMPORT_LIBRARY ${CELEDSEXPORTER_SHARED_LIBRARY})

    SET(CELLML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cellml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CELLML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CELLML_IMPORT_LIBRARY ${CELLML_SHARED_LIBRARY})

    SET(CEVAS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cevas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CEVAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CEVAS_IMPORT_LIBRARY ${CEVAS_SHARED_LIBRARY})

    SET(CUSES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}cuses${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CUSES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CUSES_IMPORT_LIBRARY ${CUSES_SHARED_LIBRARY})

    SET(MALAES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}malaes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_MALAES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(MALAES_IMPORT_LIBRARY ${MALAES_SHARED_LIBRARY})

    SET(VACSS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}vacss${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_VACSS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(VACSS_IMPORT_LIBRARY ${VACSS_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
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

SET(SHA1_FILES
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

IF(WIN32)
    LIST(APPEND SHA1_FILES
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
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_CELLMLAPI_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b78e54f55f81f17473704ed1562df14a409846ab
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES dc52651c0532b9b5d71667d127b545b15d178159
                                              f8377202dd2c444fa8292bb2795e9f05620ea72f
                                              db6891a0868df298d5baf82cf6a851c231756ff9
                                              be82406110982e6ce5abc68fb1228455d170502c
                                              4df61b9eaa3d1b43cdd98584de55b0468a1998c9
                                              01f88338089b2160a348f1042e2539529dfad597
                                              7dda7b3eb78be357770f256ed56256d11d8337b3
                                              21b9b64c78f271fd1ff36e3f9bb05cdfad851597
                                              efd5254aa9320c16cb9b0021eaf31999d5e83fd2
                                              dd9c27953a98c3d353125a7146d7144a97082260
                                              57f33bf1cf449791580fc244a9a5747ac00fb1a5
                                              d7f05b42d44da89349b5a9b38a9f592500987494
                                              399311c012059d01267a5db64b1b9e135576ca2d
                                              662813c5a48070505742739ab5cfc88cdf6cdb11
                                              c19d2cb5be0304143635061a8c4592c4e5f3faab
                                              fe1160c9f5b871e7cad7b4b7839d5d7167617ec8
                                              4ea920a2b9cc1e8e9f59903ba54cecabaf9b5d62
                                              45a5dd889fdfe4a97d274c2aea173b6ec1bd4547)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} a55bb072956b0fa2ea2ecfe39bbb8105241b3856
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b3301cb65c5e5748da2cf89a22957252c605cae9
                                              f4fe94ef4dd68bc42f71211e259dce6c700d7e97
                                              d6e8d1efbb0f6e25e9406478cd5cd646b6725699
                                              d8249aeb84e6aef03b96b98de5f76fa22a82ee65
                                              b44535aacd4f87c28b901ab3686e9c0f36581cbb
                                              6295b5fd2a249fde635f8e2861c2ca416a3b79be
                                              9acd8ae575aea3ea15bee45f64393911b341a015
                                              adfc6112849bda36222491b7fd4e96f80a7dce52
                                              0b0df93e83270c167b15b1b17ac34eea357b2e86
                                              550542f1b8b06ccc9a27dbac3f2942af7b3721ab
                                              a0a07eccb4ff1672a85f97bd7c01f32a8a21754f
                                              f33ebb87338f20438c343d050479891bc38f6361
                                              a382ab7923b644f506aaab479060ad85d8c84527
                                              1a796720e2f42780fcc20b8bfec0bf28e73f9762
                                              37716455a760b090fc5539e021f46da0baf593c4
                                              9d533f0deae4286cceba09b42c91d9f2d4c4ab6a
                                              2911ae30b2a0b80c402dbd425688dc1c44ec94d6
                                              542438e505167122f35863f59c424bea81510f85)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1708a958072452cf655206bfcc535a107f58aaf1
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 37380a773115647a87a2c3bb8aa7fc8f02182c22
                                          37e9264ab6e88d392d4f5c30d80e58ed2f293cdb
                                          c0b4920778643032150db5a54aff07f688058b90
                                          b33810f5e4f7354189218ab810bf4e33cf0a7553
                                          3cb6f6fbd91480553a5519157e47d7f2c831fa38
                                          820081bd8c190c0c40a6bfccf974278d4f9368b2
                                          baaa71a1a63cda4395d67068fe4605d1a320e2cf
                                          2467dd28a8e263e7ef898ad29725f70311e06649
                                          c5c3c5a6f5add63976c681fcd8ef4faea6c49bed)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 59d2eaf6606524d0d3b030e911f4771e78d85f94
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES dce14c38d6bfa4bd210283b148cf396f4caa7c35
                                          2506b6ffb695038ff95a6086f2e6ab1e79f70fd0
                                          944ae076222b06e5349d31289595150c34408e50
                                          402ff61a094675c8c906d3b5db847a770fde013f
                                          38f9882dd46289f8f77275822887471473857ce6
                                          61a2fffba319a2f867fe85a91966c841671f3e63
                                          48063fb2a1f9c7353fd551d45fdf616ac056cc98
                                          a9494fee39ffda2a7d38907294b7422d1f47de1a
                                          b9a453f067de6cb600559c982aa3dcefa76e45d4)
    ENDIF()
ELSE()
    # Set some compilation flags and/or ignore some warnings

    IF(WIN32)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4005 /wd4244 /wd4267 /wd4311 /wd4312")
        SET(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4477 /wd4530 /wd4577")
    ELSEIF(NOT APPLE)
        SET(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable")
    ENDIF()

    # Build the CellML API as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/cellml-api
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${PACKAGE_CXX_FLAGS}
        CMAKE_ARGS
            -DBUILD_TESTING=OFF
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
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

    IF(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyAnnotoolsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${ANNOTOOLS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}annotools${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${ANNOTOOLS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${ANNOTOOLS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCcgsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CCGS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CCGS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CCGS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCeledsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELEDS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}celeds${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELEDS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELEDS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCeledsexporterLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELEDSEXPORTER_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELEDSEXPORTER_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELEDSEXPORTER_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCellmlLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CELLML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cellml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CELLML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CELLML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCevasLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CEVAS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cevas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CEVAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CEVAS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCusesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CUSES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}cuses${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CUSES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CUSES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyMalaesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${MALAES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}malaes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MALAES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${MALAES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyVacssLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${VACSS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}vacss${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${VACSS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${VACSS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our CellML API package

SET(CELLMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(CELLMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(CELLMLAPI_ANNOTOOLS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ANNOTOOLS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CCGS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CCGS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CELEDS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CELEDS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CELEDSEXPORTER_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CELEDSEXPORTER_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CELLML_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CELLML_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CEVAS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CEVAS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_CUSES_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CUSES_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_MALAES_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${MALAES_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_VACSS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${VACSS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(CELLMLAPI_LIBRARIES
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

SET(CELLMLAPI_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/cellmlapiplugin.cpp
    HEADERS_MOC
        src/cellmlapiplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include

        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    TESTS
        tests
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
