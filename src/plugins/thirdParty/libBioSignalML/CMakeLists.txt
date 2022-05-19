project(libBioSignalMLPlugin)

# Our package characteristics

set(PACKAGE_NAME BioSignalML)
set(PACKAGE_VERSION 1.0)
set(PACKAGE_REPOSITORY libbiosignalml)
set(GIT_TAG v0.8.4)
set(RELEASE_TAG v0.8.4)

get_full_local_external_package_dir()

# Version of our package's libraries

set(MAJOR_BIOSIGNALML_LIBRARY_VERSION 0)
set(BIOSIGNALML_LIBRARY_VERSION ${MAJOR_BIOSIGNALML_LIBRARY_VERSION}.8)

set(MAJOR_TYPEDOBJECT_LIBRARY_VERSION 1)
set(TYPEDOBJECT_LIBRARY_VERSION ${MAJOR_TYPEDOBJECT_LIBRARY_VERSION}.1)

# Git tags for our libraries

set(GIT_TAG_BIOSIGNALML v0.8.4)
set(GIT_TAG_TYPEDOBJECT v1.1.2)

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

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${BIOSIGNALML_SHARED_LIBRARY}
    lib/${TYPEDOBJECT_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        lib/${BIOSIGNALML_IMPORT_LIBRARY}
        lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_BIOSIGNALML_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             e2ca2a020750760c611b72b35edfa956bc55c593)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             f01b5c1219ab792476ae6eaadf3be0329784c948)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         a9842f187002c5b9bae273bc2cd3224592f87ef2)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         b4f90f5dba99f87687b09685955e2fb4457892da)
    endif()
else()
    # Build typedobject as an external project

    ExternalProject_Add(typedObjectBuild
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/typedobject
        GIT_TAG
            ${GIT_TAG_TYPEDOBJECT}
        GIT_SHALLOW
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )

    # Build BioSignalML as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG_BIOSIGNALML}
        GIT_SHALLOW
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
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

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our BioSignalML package

set(BIOSIGNALML_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(BIOSIGNALML_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(BIOSIGNALML_BIOSIGNALML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${BIOSIGNALML_IMPORT_LIBRARY} PARENT_SCOPE)
set(BIOSIGNALML_TYPEDOBJECT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${TYPEDOBJECT_IMPORT_LIBRARY} PARENT_SCOPE)
set(BIOSIGNALML_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${BIOSIGNALML_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${TYPEDOBJECT_IMPORT_LIBRARY}
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
