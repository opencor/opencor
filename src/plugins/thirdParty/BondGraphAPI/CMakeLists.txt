project(BondGraphAPIPlugin)

# Our package characteristics

set(PACKAGE_NAME BondGraphAPI)
set(PACKAGE_VERSION 1.0)
set(PACKAGE_REPOSITORY libbondgraph)
set(GIT_TAG v1.0-opencor)
set(RELEASE_TAG v1.0-opencor)

get_full_local_external_package_dir()

# Version of our package's libraries

set(MAJOR_LIBRARY_VERSION 1)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0)


# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    set(BONDGRAPH_LIBRARY BondGraph${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BONDGRAPH_IMPORT_LIBRARY BondGraph${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(BONDGRAPH_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}BondGraph${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(BONDGRAPH_IMPORT_LIBRARY ${BONDGRAPH_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${BONDGRAPH_LIBRARY}
)

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${BONDGRAPH_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        lib/${BONDGRAPH_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

# if(USE_PREBUILT_LIBBondGraph_PACKAGE)
#     # Retrieve the plugin's package

#     if(WIN32)
#         if(RELEASE_MODE)
#             retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
#                              ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
#                              13257d84b35e18d26ae0ebe32d9b67e8065383e2)
#         else()
#             retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
#                              ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
#                              945fe15146600815e7c288e1c1465f1b55d89dd8)
#         endif()
#     elseif(APPLE)
#         retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
#                          ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
#                          e0d7e55a35fe00057673f61be4d0966c75d51901)
#     else()
#         retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
#                          ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
#                          cd70b85913725648aec063d38d9c6465ab675b9e)
#     endif()
# else()
    # Build libBondGraph as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/Jagirhussan/${PACKAGE_REPOSITORY}
        # GIT_TAG
        #     ${GIT_TAG}
        GIT_SHALLOW
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DBoost_INCLUDE_DIR=${BOOST_INCLUDE_DIR}
            -DPARSE_AND_CREATE_FACTORY_METHODS=OFF
            -DBUILD_TESTING=OFF
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${BONDGRAPH_IMPORT_LIBRARY}       
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyBGLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${BONDGRAPH_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}BondGraph${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.1.0
                                                                  ${BONDGRAPH_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    else(NOT WIN32)
        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDlls
                                COMMAND ${CMAKE_COMMAND} -E copy bin/${BONDGRAPH_LIBRARY} lib
                                WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                DEPENDEES install)
    endif(NOT WIN32)
    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
#endif()

# Allow other external projects to find our libBondGraph package

set(LIBBONDGRAPH_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(LIBBONDGRAPH_INSTALL_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/cmake PARENT_SCOPE)
set(LIBBONDGRAPH_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(LIBBONDGRAPH_LIBRARY ${EXTERNAL_BINARIES_DIR}/${BONDGRAPH_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/bondgraphapiplugin.cpp
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
