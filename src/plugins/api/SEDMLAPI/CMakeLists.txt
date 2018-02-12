project(SEDMLAPIPlugin)

# Name and version of our package

set(PACKAGE_NAME libSEDML)
set(PACKAGE_VERSION 0.4.3)

# Version of our package's libraries

set(MAJOR_LIBRARY_VERSION 0)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.4.3)

# Git tag for our library

set(GIT_TAG v0.4.3)

# Package repository

set(PACKAGE_REPOSITORY libSEDML)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    set(SHARED_LIBRARY libsedml${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY libsedml${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sedml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_SEDMLAPI_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 48929203f6cf3d58c02110f82a61200d03d37f66
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES a285b5e6532ff2ae8b68c6a47b94d911d5496b8f
                                              6fae4b4587fd0769b4d7885fdf1f59dcd753bd6a)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} db406bb2105c73742c759612dc663496d3868838
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 0e01aa3d70dc38389cf1497759bfa65d4e5d7358
                                              329e30cfc5e8e2f493e6aadf2dbd89c347867ff0)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} e94f678c588380730103eba4067e880340ddf24f
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 308705d24ebece8decf05bd014c8fe2247dc8206)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 89509c8698588a23d52c0963a181d68aeae3218f
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e20078453dd48df4be117c4efca7e6b1627a29d4)
    endif()
else()
    # Build libSEDML as an external project
    # Note: we need both libSBML and libNuML to be built, so we can access them,
    #       as well as some of libSBML's dependencies...

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    if(WIN32)
        if(RELEASE_MODE)
            set(MODE release)
        else()
            set(MODE debug)
        endif()
    endif()

    set(NUMLAPI_EXT_DIR ${CMAKE_SOURCE_DIR}/src/plugins/api/NuMLAPI/ext)
    set(SBMLAPI_EXT_DIR ${CMAKE_SOURCE_DIR}/src/plugins/api/SBMLAPI/ext)

    if(WIN32)
        set(LIBNUML_INCLUDE_DIR ${NUMLAPI_EXT_DIR}/${MODE}/include)
        set(LIBNUML_LIBRARY ${NUMLAPI_EXT_DIR}/${MODE}/lib/libnuml${CMAKE_IMPORT_LIBRARY_SUFFIX})
        set(LIBSBML_INCLUDE_DIR ${SBMLAPI_EXT_DIR}/${MODE}/include)
        set(LIBSBML_LIBRARY ${SBMLAPI_EXT_DIR}/${MODE}/lib/libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX})

        set(DEFINE_LIBZ_INCLUDE_DIR -DLIBZ_INCLUDE_DIR=${SBMLAPI_EXT_DIR}/dependencies/${MODE}/include)
        set(DEFINE_LIBZ_LIBRARY -DLIBZ_LIBRARY=${SBMLAPI_EXT_DIR}/dependencies/${MODE}/lib/zdll${CMAKE_IMPORT_LIBRARY_SUFFIX})
    else()
        set(LIBNUML_INCLUDE_DIR ${NUMLAPI_EXT_DIR}/include)
        set(LIBNUML_LIBRARY ${NUMLAPI_EXT_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}numl${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.1${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
        set(LIBSBML_INCLUDE_DIR ${SBMLAPI_EXT_DIR}/include)
        set(LIBSBML_LIBRARY ${SBMLAPI_EXT_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.5${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    endif()

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DLIBNUML_INCLUDE_DIR=${LIBNUML_INCLUDE_DIR}
            -DLIBNUML_LIBRARY=${LIBNUML_LIBRARY}
            -DLIBSBML_INCLUDE_DIR=${LIBSBML_INCLUDE_DIR}
            -DLIBSBML_LIBRARY=${LIBSBML_LIBRARY}
            ${DEFINE_LIBZ_INCLUDE_DIR}
            ${DEFINE_LIBZ_LIBRARY}
            -DWITH_CPP_NAMESPACE=ON
            -DWITH_SWIG=OFF
            -DWITH_ZLIB=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
        DEPENDS
            NuMLAPIPlugin
            SBMLAPIPlugin
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sedml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    endif()

    # Package our external project

    CREATE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our libSEDML package

set(SEDMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(SEDMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(SEDMLAPI_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin
# Note: we would normally use '${PACKAGE_NAME}' rather than 'SEDMLAPI', but it
#       can't be done here since the name of our package is different from that
#       of our plugin...

add_plugin(SEDMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/sedmlapiplugin.cpp
    HEADERS_MOC
        src/sedmlapiplugin.h
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
