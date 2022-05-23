project(libSBMLPlugin)

# Our package characteristics

set(PACKAGE_NAME libSBML)
set(PACKAGE_VERSION 5.19.0)
set(PACKAGE_REPOSITORY libsbml)
set(GIT_TAG v5.19.0)
set(RELEASE_TAG v5.19.0)

get_full_local_external_package_dir()

# Version of our package's libraries

set(MAJOR_LIBRARY_VERSION 5)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.19.0)

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    set(SHARED_LIBRARY libsbml${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

if(USE_PREBUILT_LIBSBML_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             eb16c13181c35d7c9c5ff2a8b848127359668fda)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             0402549c9be74541589007232ef974c2d59808a3)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         bb62c94856fc7294dab3a9ffd39e670428a90306)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         e27ea428b40a2e5187264e4cd131af7b3b5c3346)
    endif()
else()
    # Download some dependencies for Windows, if needed
    # Note: they can originally be found at
    #       https://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/...

    if(WIN32)
        if(RELEASE_MODE)
            set(MODE release)
        else()
            set(MODE debug)
        endif()

        set(DEPENDENCIES_FILENAME libSBML_dependencies_vs15_${MODE}_x64.zip)
        set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/src/plugins/thirdParty/libSBML/ext/dependencies)
        set(DEPENDENCIES_MODE_DIR ${DEPENDENCIES_DIR}/${MODE})

        if(NOT EXISTS ${DEPENDENCIES_MODE_DIR})
            message("Retrieving the 'libSBML' dependencies...")

            set(REAL_DEPENDENCIES_FILENAME ${DEPENDENCIES_DIR}/${DEPENDENCIES_FILENAME})

            file(DOWNLOAD "https://github.com/opencor/${PACKAGE_REPOSITORY}/releases/download/${GIT_TAG}/${DEPENDENCIES_FILENAME}" ${REAL_DEPENDENCIES_FILENAME}
                 SHOW_PROGRESS STATUS STATUS)

            list(GET STATUS 0 STATUS_CODE)

            if(${STATUS_CODE} EQUAL 0)
                execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${REAL_DEPENDENCIES_FILENAME}
                                WORKING_DIRECTORY ${DEPENDENCIES_DIR}
                                OUTPUT_QUIET)
                execute_process(COMMAND ${CMAKE_COMMAND} -E rename "libSBML Dependencies-1.0.0-b1-win64" ${MODE}
                                WORKING_DIRECTORY ${DEPENDENCIES_DIR}
                                OUTPUT_QUIET)

                file(REMOVE ${REAL_DEPENDENCIES_FILENAME})
            else()
                file(REMOVE ${REAL_DEPENDENCIES_FILENAME})
                # Note: this is in case we had an HTTP error of sorts, in which case we
                #       would end up with an empty file...

                message(FATAL_ERROR "The libSBML depencies could not be retrieved...")
            endif()
        endif()
    endif()

    # Ignore some warnings

    if(APPLE)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-switch")
    endif()

    # Build libSBML as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DLIBSBML_DEPENDENCY_DIR=${DEPENDENCIES_MODE_DIR}
            -DWITH_BZIP2=OFF
            -DWITH_CPP_NAMESPACE=ON
            -DWITH_SWIG=OFF
            -DWITH_ZLIB=OFF
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDll
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    endif()

    # Package our external project

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our libSBML package

set(LIBSBML_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(LIBSBML_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(LIBSBML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/libsbmlplugin.cpp
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
