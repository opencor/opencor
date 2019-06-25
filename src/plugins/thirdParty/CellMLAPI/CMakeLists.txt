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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 036c8fa4f2e1a2c954986866879d4d79516a9068
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 4db6996a4451d1e6f33d0936e7a8f2c8ce0c226f
                                              989d9f0521f45e32ac394bedb26a30611dadbe7a
                                              3d0ad5a0d3750f034aec31fd79dec9d54dab1f1c
                                              7d22f75f0dc1688fbbe3855faa70458151f86c83
                                              b9db37975c7d4ab13102b76d75fea5134d012cc4
                                              5aa7e17afe43c5057d600c2e3bfb7dd39b6136b6
                                              5ab85fb05cc552047053cb21597a85b26102cf3a
                                              d9e8a46b594a35c8444702b7fa91d045c1298fba
                                              50733dc364ae9328163673f27a2d1a478976e5c7
                                              af5e5b2af39baf1bc22b063c21b0aefab3b35ce7
                                              09844649086304c2baf4d81074eb4bd692e84836
                                              10610205814740d1cbeb1b97ae69d07ff919664e
                                              a04a78e53821f98658193739bd69b24d2e659909
                                              03881e17dc36a4469350d2cc2632eecd461106df
                                              1cc906956732a21fa65ac54069495220766330c3
                                              8fd5b592d8a4eb4a6070861dcad80b31e7c738c8
                                              afc3196b3d4f6093fd2f7fa4117377f25bd24558
                                              59725a3835fa48fec41e64ba750a991abeca6f02)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} bbeab36f10ff7c95e5ddaa8239f66ba893a9798b
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 764f7c4c609edde15d003fbaa0fe558808e9598a
                                              e89a375c2b733dda530d518159de0f4613e2571c
                                              b7bb749b6131336edde0ab6f2046aae46ebcd108
                                              b75971b436563f54c7e6cdeb363549cf204fad3d
                                              80ca0fb789b168644173819752c48bf44bfaf0d4
                                              67970ff88619cdb5f34c86734e4049ff7cf91524
                                              cdff4999cbe7eb9f5a6c46bc508df4d3f59752f5
                                              c0fd4c03d35642dfbed3e00877788d442710db5b
                                              46f7b9190cc5484bfed9abe9c0b14c7abd3fd64f
                                              585b050c72c3a6adba15973001059cb0a8bff71b
                                              2346c3ffe9deafddea53e82fda34d26956bf821b
                                              1515276b59ded83b3889c2110178ea9bb2605d6a
                                              551095e12ebf10d26563f9770173e8ced450269d
                                              809a4de9397b96cc38d7ec95ac599e5ea82f63cd
                                              9a1435eb8cc323a0d73496469fc373c61aa0874d
                                              5514f543c45e03dbc2418974a493eaaff5cc8560
                                              ddeab87421ebd810576bec2c4f1d55f866af23e0
                                              3777514941def210af9a50138a3615dede71fdf0)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 21cd963f01ae3e44ce89b380afe1f55e85feb263
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 68b7129fab37f6fe6593563ab9165864665173e7
                                          611978ebe2a599a5b2c4c91830155bdb532c1c7a
                                          35848e330b50fbb8649d4be92f59dd14b8d153e4
                                          214c2438603f1ffb1548e577d369784b4f20472e
                                          357df21d83aa4f6d06de0aaec7044d8e22e48928
                                          1333bde2bac7fefb431002c6ac7de6d644c84b1d
                                          3ec0308673a528e536cd28fa21ecac8f6486d40d
                                          96a683a58f2ae7cd62df1d953220fc2ac06039f6
                                          d8e0879cf3d30692fc4c89059a1e4cc685097487)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} cfe1eaf66f3c22a3a095e8a90c7c8bf98db86dc1
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 43889db939b2d6827da653213cd6f66aee712d54
                                          31890f300a1df480900760b30f0e5b24cf0fc67a
                                          dae15535a1b35add6bc3ef0a8b6876e1843cf2cc
                                          08b5b25b9d058023dc120f46eea72ef7805a33fd
                                          e615434aad59a9e90be8c4f45206a2c157cfab9e
                                          f383687f9b159f2796e89241646ec7bad161bee0
                                          0a46ae93be3c0d0790e3cc48e4f41b73cec26987
                                          288100ad0af7dd51600a98edc2c250708f10057c
                                          c7aad25e0d35c3291981b9e60b792b08c577e251)
    endif()
else()
    # Set some compilation flags and/or ignore some warnings

    if(WIN32)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4005 /wd4244 /wd4267 /wd4311 /wd4312")
        set(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4267 /wd4477 /wd4530 /wd4577")
    elseif(NOT APPLE)
        set(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable")
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
