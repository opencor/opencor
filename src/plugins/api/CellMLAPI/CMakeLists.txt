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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b56132b99c3e34ef55b29050e7b120477b540fb7
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9a4cc1858159c1d94a2f4903b475c94c01b7a4e4
                                              7b51afcf1f41860fbe1f49f00f7697e8a5cd99b5
                                              6b6a0f6738552376919ea8361763e95e2d111a69
                                              df00a32ebba74e9fc1ea7514a1b673e38707fbe8
                                              1168550b38cbb21c7be6212b5816950facb5c509
                                              9aab67a6ced0154f8138ebb8603a590d7a948b06
                                              3c12d9755c92033ca0a8be07c456e4bc955945d5
                                              6a8dc7bbdb3ead4c79bfebdabd068dd0338b3a90
                                              248b0de4ea72f4039486a89c83df94a2077d2756
                                              37a9094bc9c115ec4c1caf969ecf1c3b0d38b341
                                              01d643c393032c5531c3fd028cf811bd0b9cf2f3
                                              608777bce46fc7b618d3b6b2210a5408f703787e
                                              7329d5ba7457c67a065852e546285722ebd24a98
                                              44fa4da29fe7b860889a46ae7e9fef22fb99763a
                                              2321414a5d222afe1f9ae748b9f95a913730cff6
                                              11cc047c0bd5ba1787df8790cfc8ae5e2b486b9c
                                              c7240bc6f89e57e489e16c757d209a8e7ce51758
                                              7915bd55347d91c9d5636ec67a7e27cbe2de2df6)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 361d744df1e868798b24d48ae62a68273ad97709
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES f40a1a1d3d79c56d413076135f3000e9ba3b23ae
                                              1432e5d117bdab34546f8dba1e88a0e089e5b189
                                              599c2734f0dca7395226d683a0cb4cda1a0904c6
                                              a16f3bce6ef9193c57b191de615e47212195ed21
                                              7589962c91d79833ca33dec1d43755a8b2e4ba09
                                              d55020892796585c515a9667f8570f236732da60
                                              2203e75c4c5c997f1049cfbe6d53ce10085fd6f7
                                              3bd91da76a9563fcaa700d14aa499c75e030c94b
                                              c6103bafccc26169dc5f81ce1df0c16f0e77b54e
                                              2627604addc53caada2fcd544c34d20e9d058ffb
                                              34b1868b55a8707f81b0c6386288d4402e3eb4b7
                                              bc0cfccac187f4fecdf0698b8aa42cde7421f877
                                              34ce015e75e002b4261e8fbdaa5d7a012094a751
                                              1b024b6cf3b65b86db816b9b19bf365210272f2a
                                              681d7aeda73057972712cf02b1cb6ff58e5c0e6f
                                              b14de8c59650c5f311799cef9fa573ab143bf830
                                              90b1c56d36c9b50767e033c25f886e273a7e3034
                                              278b0aa29730c97ee27cdc9d64dd8339e6f3c23e)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 5046db57bc3bf1af2daff77631311c04767fc020
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
                              ${RELATIVE_PROJECT_SOURCE_DIR} 192f9ec884bfb1bc33ea2460994dbbd1e4cde03d
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 49224745c95cd278958881d8d572c85171364dda
                                          b9aa16ec5fe313d1e2b9e541108caff8cbec1cc7
                                          026cbe3e1c2e5ae854656754f9ace487ebe3ada1
                                          8d78cb1e721007132b482f7e383cca53ca702a22
                                          a1b532baf66825ec61a1467bd8611bedb693d1ce
                                          511578c9799f11bb70d3a78aaacf141b6605376b
                                          3681647085696a2e139b048b698647abc19ba497
                                          6084a26b5c09831daf6d39e018a8c5025e9e98d8
                                          193184a48fa5ec1d061dcdd19406d91ae18247e1)
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
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/cellml-api
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${PACKAGE_CXX_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DBUILD_TESTING=OFF
            ${DEFINE_CMAKE_APPLE_OPTIONS}
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

    SET(DEPENDS_ON ${PACKAGE_BUILD})
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

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/cellmlapiplugin.cpp
    HEADERS_MOC
        src/cellmlapiplugin.h
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
