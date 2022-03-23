project(ZincPlugin)

# Name and version of our package

set(PACKAGE_NAME Zinc)
set(PACKAGE_VERSION 3.1.2)

# Version of our library

set(MAJOR_LIBRARY_VERSION 3)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.2)

# Git tag for our library

set(GIT_TAG v3.1.2)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    if(RELEASE_MODE)
        set(DEBUG_TAG)
    else()
        set(DEBUG_TAG d)
    endif()

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

if(USE_PREBUILT_ZINC_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} d7e08929a968a661bf8e9f00132929a4f3e244fb
                             SHA1_FILES ${SHA1_FILES}
                             SHA1_VALUES 351e90a846ad08149c8799dc0888f4083a6f7ccc
                                         7a8d19846bbc3fd281ec7f98581ea34174ce278a)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} dfe1440281818d30e0c38893b3824a5493f00222
                             SHA1_FILES ${SHA1_FILES}
                             SHA1_VALUES 56c8d7c66b402561310b67e3e84f0efdac4b2bdf
                                         04691b8f3b0f1fdb89d12e29ceb75bd23e7d60f3)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} fe7eec086dab717648db9de40aba27c884391ba6
                         SHA1_FILES ${SHA1_FILES}
                         SHA1_VALUES 92e2830c0aca29686c32485e168e8ce79e12b81e)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 5823b182beffc0f62d6847685f8d2a0643adf692
                         SHA1_FILES ${SHA1_FILES}
                         SHA1_VALUES c2fce021cdecd03e24b43d0ae2a1c8ead65e1a3a)
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
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}/setup
        BINARY_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}/setup-build
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/zinc
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
                   PACKAGED_FILES include ${SHA1_FILES}
                   SHA1_FILES ${SHA1_FILES}
                   TARGET ${PACKAGE_BUILD})

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
