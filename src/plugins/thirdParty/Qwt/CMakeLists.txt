project(QwtPlugin)

# Name and version of our package

set(PACKAGE_NAME Qwt)
set(PACKAGE_VERSION 6.1.3)

# Version of our library

set(MAJOR_LIBRARY_VERSION 6)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.3)

# Git tag for our library

set(GIT_TAG v6.1.3)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    if(RELEASE_MODE)
        set(DEBUG_TAG)
    else()
        set(DEBUG_TAG d)
    endif()

    set(QWT_SHARED_LIBRARY qwt${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWT_IMPORT_LIBRARY qwt${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(QWTMATHML_SHARED_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(QWTMATHML_IMPORT_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(QWT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWT_IMPORT_LIBRARY ${QWT_SHARED_LIBRARY})

    set(QWTMATHML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(QWTMATHML_IMPORT_LIBRARY ${QWTMATHML_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${QWT_SHARED_LIBRARY}
    ${QWTMATHML_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${QWT_SHARED_LIBRARY}
    lib/${QWTMATHML_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${QWT_IMPORT_LIBRARY}
        lib/${QWTMATHML_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_QWT_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e3a45a41d562b88652048323d8c019213dc71b8a
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b5bda4086109a1ae681a81f33c45738029116af5
                                              f880c425fe49514eaae2b6dbfa75fc2aece9a46a
                                              4c712b690f0cce2a87624b3abcb4557fed58493f
                                              d603b5b1c7f4f342388369904129e874db0d7f4b)
        else()
            RETRIEVE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e2730a8271713f73451478284f6f7d564dbff0a3
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES ff70d89f051b4e30df4ed26d38a55fc5cd6adc10
                                              a606dbacd9df38411f871b2b5423168a427f3469
                                              73d02811cf131145b46f597231f438813c8eb01b
                                              c405859c00ad588bd6bf65bb3c270c6ea484f07d)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 8e4facd5340065c885fa6634eb83684aab1acead
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2f2341decf3a24faedb6f4e83165226266eba5f9
                                          d5787ce81a8958a4a7453cc9a95cf77222d08a49)
    else()
        RETRIEVE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} c5c7e6ac1cad4d419345b3f38fbd553fa84f6194
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c08325652c065ae05d451c7e45e63befd0c39199
                                          f46e2fa826ffdcc89cfea705b846263f9590f181)
    endif()
else()
    # Set platform specific configuration options

    if(WIN32)
        if(RELEASE_MODE)
            set(MAKEFILE_TARGET release)
        else()
            set(MAKEFILE_TARGET debug)
        endif()
    endif()

    # Build Qwt as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qwt
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} <SOURCE_DIR> CONFIG+=qt_no_framework OBJECTS_DIR=${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_JOM_COMMAND} ${MAKEFILE_TARGET}
        INSTALL_COMMAND
            ${MAKE_JOM_COMMAND} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${QWT_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${QWTMATHML_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyQwtLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyQwtmathmlLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWTMATHML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWTMATHML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        if(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${QWT_SHARED_LIBRARY}
                                                                       @rpath/${QWT_SHARED_LIBRARY}
                                                                       ${QWTMATHML_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES CopyQwtmathmlLibrary)
        endif()
    endif()

    # Package our external project

    CREATE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our Qwt package

set(QWT_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(QWT_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(QWT_QWT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY} PARENT_SCOPE)
set(QWT_QWTMATHML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY} PARENT_SCOPE)
set(QWT_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qwtplugin.cpp
    HEADERS_MOC
        src/qwtplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
