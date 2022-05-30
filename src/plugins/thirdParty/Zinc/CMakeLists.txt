project(ZincPlugin)

# Our package characteristics

set(PACKAGE_NAME Zinc)
set(PACKAGE_VERSION 3.1.2)
set(PACKAGE_REPOSITORY zinc)
set(GIT_TAG v3.1.2)
set(RELEASE_TAG v3.1.2)

get_full_local_external_package_dir()

# Version of our package's library

set(MAJOR_LIBRARY_VERSION 3)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.2)

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(SHARED_LIBRARY zinc${DEBUG_TAG}.${LIBRARY_VERSION}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY zinc${DEBUG_TAG}.${LIBRARY_VERSION}${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}zinc${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_ZINC_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             d7e08929a968a661bf8e9f00132929a4f3e244fb)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             dfe1440281818d30e0c38893b3824a5493f00222)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         fe7eec086dab717648db9de40aba27c884391ba6)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         5823b182beffc0f62d6847685f8d2a0643adf692)
    endif()
else()
    # Set platform specific configuration options

    if(WIN32)
        math(EXPR MSVC_VERSION "${CMAKE_CXX_COMPILER_VERSION_MAJOR}-4")

        set(DEFINE_CMAKE_GENERATOR -G "Visual Studio 14 20${MSVC_VERSION} Win64")
        set(SHARED_lIBRARY_DIR bin)
    else()
        set(DEFINE_CMAKE_GENERATOR)
        set(SHARED_lIBRARY_DIR lib)
    endif()

    # Build Zinc as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CONFIGURE_COMMAND
            ${CMAKE_COMMAND} ${DEFINE_CMAKE_GENERATOR} ${CMAKE_ARGS} -DOPENCMISS_ROOT=<BINARY_DIR>/.. -DOPENCMISS_LIBRARIES=zinc <SOURCE_DIR>
        BUILD_COMMAND
            ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
        INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/../install/include/opencmiss <INSTALL_DIR>/include/opencmiss
            && ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/../install/${SHARED_lIBRARY_DIR}/${SHARED_LIBRARY} <INSTALL_DIR>/lib
            && ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/../install/lib/${IMPORT_LIBRARY} <INSTALL_DIR>/lib
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our Zinc package

set(ZINC_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(ZINC_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(ZINC_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/zincplugin.cpp
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
