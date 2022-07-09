project(BondGraphUIWidgetsPlugin)

# Our package characteristics

set(PACKAGE_NAME BondGraphUIWidgets)
set(PACKAGE_VERSION 1.0.0)
set(PACKAGE_REPOSITORY BondGraphUIWidgets)
set(GIT_TAG v1.0.0)
set(RELEASE_TAG v1.0.0)

get_full_local_external_package_dir()

# Version of our package's library
set(WIDGETLIBNAME BondGraphUI)
set(MAJOR_LIBRARY_VERSION 1)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0.0)

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(SHARED_LIBRARY ${WIDGETLIBNAME}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${WIDGETLIBNAME}${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX}) 
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}${WIDGETLIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()


# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()


#     # Set platform specific configuration options

if(WIN32)
    math(EXPR MSVC_VERSION "${CMAKE_CXX_COMPILER_VERSION_MAJOR}-4")

    set(DEFINE_CMAKE_GENERATOR -G "Visual Studio 14 20${MSVC_VERSION} Win64")
    set(SHARED_lIBRARY_DIR bin)
else()
    set(DEFINE_CMAKE_GENERATOR)
    set(SHARED_lIBRARY_DIR lib)
endif()

# Build BondGraphUI as an external project

set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

#BondGraphUI depends on BondGraph library

ExternalProject_Add(${PACKAGE_BUILD}
    INSTALL_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    # GIT_REPOSITORY
    #     https://github.com/opencor/${PACKAGE_REPOSITORY}
    # GIT_TAG
    #     ${GIT_TAG}
    # GIT_SHALLOW
    SOURCE_DIR /home/rjag008/bguistandalone        
    CMAKE_GENERATOR
        ${CMAKE_GENERATOR}
    CMAKE_ARGS
        ${CMAKE_ARGS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DBondGraph_DIR=${LIBBONDGRAPH_INSTALL_DIR}
    BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    DEPENDS
        BondGraphAPIPlugin
)


# Do some post-processing that is specific to each platform

# Package our external project

create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                include ${STRIPPABLE_FILES})

set(DEPENDS_ON ${PACKAGE_BUILD})


# Allow other external projects to find our BondGraphUI package

set(LIBBONDGRAPHUI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(LIBBONDGRAPHUI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(LIBBONDGRAPHUI_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/bondgraphuiwidgetsplugin.cpp
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
