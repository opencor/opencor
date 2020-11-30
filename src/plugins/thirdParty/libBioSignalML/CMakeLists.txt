project(libBioSignalMLPlugin)

# Name and version of our package

set(PACKAGE_NAME BioSignalML)
set(PACKAGE_VERSION 1.0)

# Version of our package's libraries

set(MAJOR_BIOSIGNALML_LIBRARY_VERSION 0)
set(BIOSIGNALML_LIBRARY_VERSION ${MAJOR_BIOSIGNALML_LIBRARY_VERSION}.8)

set(MAJOR_TYPEDOBJECT_LIBRARY_VERSION 1)
set(TYPEDOBJECT_LIBRARY_VERSION ${MAJOR_TYPEDOBJECT_LIBRARY_VERSION}.1)

# Git tags for our libraries

set(GIT_TAG_BIOSIGNALML v0.8.4)
set(GIT_TAG_TYPEDOBJECT v1.1.2)

# Package repository and release tags

set(PACKAGE_REPOSITORY libbiosignalml)
set(RELEASE_TAG v0.8.4)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(BIOSIGNALML_SHARED_LIBRARY biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BIOSIGNALML_IMPORT_LIBRARY biosignalml${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(TYPEDOBJECT_SHARED_LIBRARY typedobject${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(TYPEDOBJECT_IMPORT_LIBRARY typedobject${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(BIOSIGNALML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_BIOSIGNALML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(BIOSIGNALML_IMPORT_LIBRARY ${BIOSIGNALML_SHARED_LIBRARY})

    set(TYPEDOBJECT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}typedobject${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_TYPEDOBJECT_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(TYPEDOBJECT_IMPORT_LIBRARY ${TYPEDOBJECT_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${BIOSIGNALML_SHARED_LIBRARY}
    ${TYPEDOBJECT_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${BIOSIGNALML_SHARED_LIBRARY}
    lib/${TYPEDOBJECT_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${BIOSIGNALML_IMPORT_LIBRARY}
        lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_BIOSIGNALML_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} e2ca2a020750760c611b72b35edfa956bc55c593
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 379e38afc9eb015b54cd327aac0d725f25fb95fc
                                              c18a129f49429083c40c3621f61256ddcbe83c05
                                              c4bf89a74703d73220e7b8be29944c409fca1a06
                                              a10d81c681fae3efc41d7855c4250f7333821256)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} f01b5c1219ab792476ae6eaadf3be0329784c948
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 6ffeea24b14d9769e5b50aed663ce5091cd43101
                                              136f63617ad96a272d56b0e13f3d0b090f096d75
                                              0dcde52f7037472d6d642e71f9e616f0fac18095
                                              556aa50cf56cd0e934a5cd7c392cef4be1d01d1c)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} a9842f187002c5b9bae273bc2cd3224592f87ef2
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 7141316b23d7cace02e72466808f6710ad40a4b8
                                          8e6ca96135ae670a53422a44827579740108d559)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} b4f90f5dba99f87687b09685955e2fb4457892da
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES fac5d6e21d05f08ec614610aca6ef967d8e53fb6
                                          cbe820dd0e2c5a79a1bb6adddd8cd00c11accb38)
    endif()
else()
    # Build typedobject as an external project

    ExternalProject_Add(typedObjectBuild
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/typedobject
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/typedobject
        GIT_TAG
            ${GIT_TAG_TYPEDOBJECT}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )

    # Build BioSignalML as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG_BIOSIGNALML}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -Dtypedobject_DIR=<INSTALL_DIR>/lib/cmake/typedobject
            -Dtypedobject_INCLUDE_DIR=<INSTALL_DIR>/include
            -Dtypedobject_LIBRARY=<INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${BIOSIGNALML_IMPORT_LIBRARY}
        DEPENDS
            typedObjectBuild
    )

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDlls
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${BIOSIGNALML_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${TYPEDOBJECT_SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyLibraries
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${BIOSIGNALML_SHARED_LIBRARY}
                                                                       ${TYPEDOBJECT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${BIOSIGNALML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${BIOSIGNALML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}typedobject${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${TYPEDOBJECT_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${TYPEDOBJECT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
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

# Allow other external projects to find our BioSignalML package

set(BIOSIGNALML_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(BIOSIGNALML_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(BIOSIGNALML_BIOSIGNALML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${BIOSIGNALML_IMPORT_LIBRARY} PARENT_SCOPE)
set(BIOSIGNALML_TYPEDOBJECT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${TYPEDOBJECT_IMPORT_LIBRARY} PARENT_SCOPE)
set(BIOSIGNALML_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${BIOSIGNALML_BIOSIGNALML_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${BIOSIGNALML_TYPEDOBJECT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin
# Note: we would normally use '${PACKAGE_NAME}' rather than 'OAuth', but it
#       can't be done here since the name of our package is different from that
#       of our plugin...

add_plugin(libBioSignalML
    SOURCES
        ../../plugininfo.cpp

        src/libbiosignalmlplugin.cpp
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
