MACRO(INITIALISE_PROJECT)
#    SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Make sure that we are using the compiler we support

    IF(WIN32)
        STRING(REGEX REPLACE "\\..*$" ""
               CMAKE_CXX_COMPILER_VERSION_MAJOR "${CMAKE_CXX_COMPILER_VERSION}")

        IF(   NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"
           OR NOT CMAKE_CXX_COMPILER_VERSION_MAJOR VERSION_EQUAL "19")
            MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using MSVC 2015 on Windows...")
        ENDIF()
    ELSEIF(APPLE)
        IF(    NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
           AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
            MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using (Apple) Clang on macOS...")
        ENDIF()
    ELSE()
        IF(   NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
           OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
            MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using GCC/G++ 4.9+ on Linux...")
        ENDIF()
    ENDIF()

    # Make sure that we are building on a supported architecture
    # Note: normally, we would check the value of CMAKE_SIZEOF_VOID_P, but in
    #       some cases it may not be set (e.g. when generating an Xcode project
    #       file), so we determine and retrieve that value ourselves...

    TRY_RUN(ARCHITECTURE_RUN ARCHITECTURE_COMPILE
            ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/cmake/architecture.c
            RUN_OUTPUT_VARIABLE ARCHITECTURE)

    IF(NOT ARCHITECTURE_COMPILE)
        MESSAGE(FATAL_ERROR "We could not determine your architecture. Please clean your ${CMAKE_PROJECT_NAME} environment and try again...")
    ELSEIF(NOT ${ARCHITECTURE} EQUAL 64)
        MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in 64-bit mode...")
    ENDIF()

    # By default, we are building a release version of OpenCOR, unless we are
    # explicitly asked for a debug version

    IF(   "${CMAKE_BUILD_TYPE}" STREQUAL ""
       OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        SET(BUILD_INFORMATION "Building a release version of ${CMAKE_PROJECT_NAME}")

        SET(RELEASE_MODE TRUE)
    ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        SET(BUILD_INFORMATION "Building a debug version of ${CMAKE_PROJECT_NAME}")

        SET(RELEASE_MODE FALSE)
    ELSE()
        MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in release or debug mode...")
    ENDIF()

    # Make sure that OpenSSL is available on Linux and macOS
    # Note: it's currently needed for libgit2, but it might be needed for other
    #       things too in the future, so make sure it's available...

    IF(NOT WIN32)
        FIND_PACKAGE(OpenSSL REQUIRED QUIET)
    ENDIF()

    # Required Qt modules and packages

    IF(ENABLE_TESTS)
        SET(TEST Test)
    ELSE()
        SET(TEST)
    ENDIF()

    SET(REQUIRED_QT_MODULES
        Network
        ${TEST}
        Widgets
    )

    FOREACH(REQUIRED_QT_MODULE ${REQUIRED_QT_MODULES})
        FIND_PACKAGE(Qt5${REQUIRED_QT_MODULE} REQUIRED)
    ENDFOREACH()

    # Make sure that anyone can access diff-match-patch

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/3rdparty/diff_match_patch/src)

    # Some initialisations related to our copy of QtWebKit

    IF(WIN32)
        SET(PLATFORM_DIR windows)
    ELSEIF(APPLE)
        SET(PLATFORM_DIR macos)
    ELSE()
        SET(PLATFORM_DIR linux)
    ENDIF()

    INCLUDE(${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit/CMakeLists.txt)

    # The WebKit module is also needed on Windows

    IF(WIN32)
        LIST(APPEND REQUIRED_QT_MODULES WebKit)

        FIND_PACKAGE(Qt5WebKit REQUIRED)
    ENDIF()

    # Keep track of some information about Qt

    SET(QT_VERSION ${Qt5Core_VERSION})
    SET(QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR})

    SET(QT_DIR ${_qt5Core_install_prefix})
    SET(QT_BINARY_DIR ${QT_DIR}/bin)
    SET(QT_LIBRARY_DIR ${QT_DIR}/lib)
    SET(QT_PLUGINS_DIR ${QT_DIR}/plugins)

    # On macOS, keep track of the Qt libraries against which we need to link

    IF(APPLE)
        IF(ENABLE_TESTS)
            SET(QT_TEST QtTest)
        ELSE()
            SET(QT_TEST)
        ENDIF()

        SET(MACOS_QT_LIBRARIES
            QtCLucene
            QtConcurrent
            QtCore
            QtGui
            QtHelp
            QtMacExtras
            QtMultimedia
            QtMultimediaWidgets
            QtNetwork
            QtOpenGL
            QtPositioning
            QtPrintSupport
            QtQml
            QtQuick
            QtSensors
            QtSql
            QtSvg
            ${QT_TEST}
            QtWebChannel
            QtWebKit
            QtWebKitWidgets
            QtWidgets
            QtXml
            QtXmlPatterns
        )
    ENDIF()

    # Determine the effective build directory

    SET(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR})

    IF(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
        # With Xcode, we have a configuration directory, but it messes up our
        # build system, so ask for all the binaries to be generated in our build
        # folder

        SET(XCODE TRUE)

        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})

        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})

        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
    ELSE()
        # Check whether there is a configuration directory (the case with MSVC)
        # and, if so, make use of it

        SET(XCODE FALSE)

        IF(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
            SET(PROJECT_BUILD_DIR ${PROJECT_BUILD_DIR}/${CMAKE_CFG_INTDIR})
        ENDIF()
    ENDIF()

    # Some general build settings
    # Note: MSVC enables C++11 support by default, so we just need to enable it
    #       on Linux and macOS...

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /WX /GR /EHsc")
        # Note: MSVC has a /Wall flag, but it results in MSVC being very
        #       pedantic, so instead we use what MSVC recommends for production
        #       code, which is /W3 and which is also what CMake uses by
        #       default...

        SET(LINK_FLAGS_PROPERTIES "/STACK:10000000")
    ELSE()
        SET(CMAKE_CXX_FLAGS "-Wall -W -Werror -std=c++11")

        IF(APPLE)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            SET(LINK_FLAGS_PROPERTIES "-stdlib=libc++")
        ELSE()
            SET(LINK_FLAGS_PROPERTIES)
        ENDIF()
    ENDIF()

    # On macOS, we want to be able to access Cocoa

    IF(APPLE)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -framework AppKit")
    ENDIF()

    # Some build settings that depend on whether we want a release or a debug
    # version of OpenCOR

    IF(RELEASE_MODE)
        # Default compiler and linker settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /MD /O2 /Ob2")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
        ENDIF()

        IF(NOT WIN32 AND NOT APPLE)
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
            # Note #1: -Wl,-s strips all the symbols, thus reducing the final
            #          size of OpenCOR or one its shared libraries...
            # Note #2: the above linking option has become obsolete on macOS...
        ENDIF()

        # Make sure that debugging is off in Qt

        ADD_DEFINITIONS(-DQT_NO_DEBUG)
    ELSE()
        # Default compiler and linker settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /DEBUG")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        ENDIF()

        # Make sure that debugging is on in Qt

        ADD_DEFINITIONS(-DQT_DEBUG)

        # Make sure that LLVM works fine when in debug mode
        # Note: it needs to be done here since, by default, _DEBUG_POINTER_IMPL
        #       is not defined and will therefore be as soon as we include
        #       xutility, which will also give it the value _Debug_pointer while
        #       we want it to be defined but without any value (so that pointer
        #       testing is disabled)...

        ADD_DEFINITIONS(-D_DEBUG_POINTER_IMPL=)
    ENDIF()

    # Disable a warning that occurs on (the 64-bit version of) Windows
    # Note: the warning occurs in (at least) MSVC's algorithm header file. To
    #       disable it here means that we disable it for everything, but is
    #       there another solution?...

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267")
    ENDIF()

    # Ask for Qt deprecated uses to be reported

    ADD_DEFINITIONS(-DQT_DEPRECATED_WARNINGS)

    # Let OpenCOR know about the options with which it was built

    IF(ENABLE_SAMPLE_PLUGINS)
        ADD_DEFINITIONS(-DENABLE_SAMPLE_PLUGINS)
    ENDIF()

    IF(ENABLE_TEST_PLUGINS)
        ADD_DEFINITIONS(-DENABLE_TEST_PLUGINS)
    ENDIF()

    IF(ENABLE_TESTS)
        ADD_DEFINITIONS(-DENABLE_TESTS)
    ENDIF()

    IF(USE_PREBUILT_LIBGIT2_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_LIBGIT2_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_LIBXDIFF_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_LIBXDIFF_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_LLVM_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_LLVM_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_QSCINTILLA_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_QSCINTILLA_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_QWT_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_QWT_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_SUNDIALS_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_SUNDIALS_PLUGIN)
    ENDIF()

    IF(USE_PREBUILT_ZLIB_PLUGIN)
        ADD_DEFINITIONS(-DUSE_PREBUILT_ZLIB_PLUGIN)
    ENDIF()

    # On macOS, make sure that we support 10.8 and later, unless a specific
    # deployment target has been specified

    IF(APPLE)
        IF("${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
            SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.8)
        ENDIF()

        IF(CMAKE_OSX_DEPLOYMENT_TARGET VERSION_LESS "10.12")
            SET(SYSTEM_NAME "OS X")
        ELSE()
            SET(SYSTEM_NAME "macOS")
        ENDIF()

        SET(BUILD_INFORMATION "${BUILD_INFORMATION} for ${SYSTEM_NAME} ${CMAKE_OSX_DEPLOYMENT_TARGET} and later")
    ENDIF()

    # Destination of our plugins so that we don't have to deploy OpenCOR on
    # Windows and Linux before being able to test it

    IF(APPLE)
        SET(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${CMAKE_PROJECT_NAME})
    ELSE()
        SET(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/plugins/${CMAKE_PROJECT_NAME})
    ENDIF()

    # Default location of external binaries and packages

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR}/release)
            SET(LOCAL_EXTERNAL_BINARIES_DIR bin/release)

            SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR}/release)
            SET(LOCAL_EXTERNAL_PACKAGE_DIR ext/release)
        ELSE()
            SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR}/debug)
            SET(LOCAL_EXTERNAL_BINARIES_DIR bin/debug)

            SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR}/debug)
            SET(LOCAL_EXTERNAL_PACKAGE_DIR ext/debug)
        ENDIF()

        SET(DEST_EXTERNAL_BINARIES_DIR bin)
    ELSE()
        SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR})
        SET(LOCAL_EXTERNAL_BINARIES_DIR bin)

        SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR})
        SET(LOCAL_EXTERNAL_PACKAGE_DIR ext)

        IF(APPLE)
            SET(DEST_EXTERNAL_BINARIES_DIR ${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)
        ELSE()
            SET(DEST_EXTERNAL_BINARIES_DIR lib)
        ENDIF()
    ENDIF()

    SET(FULL_DEST_EXTERNAL_BINARIES_DIR ${PROJECT_BUILD_DIR}/${DEST_EXTERNAL_BINARIES_DIR})

    IF(NOT EXISTS ${FULL_DEST_EXTERNAL_BINARIES_DIR})
        FILE(MAKE_DIRECTORY ${FULL_DEST_EXTERNAL_BINARIES_DIR})
    ENDIF()

    IF(APPLE)
        SET(FULL_DEST_EXTERNAL_BASE_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR})
    ELSE()
        SET(FULL_DEST_EXTERNAL_BASE_DIR ${PROJECT_BUILD_DIR})
    ENDIF()

    # Set the RPATH (and RPATH link, if needed) information on Linux and macOS

    IF(APPLE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

        SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks;@executable_path/../PlugIns/${CMAKE_PROJECT_NAME}")
    ELSEIF(NOT WIN32)
        SET(CMAKE_SKIP_RPATH TRUE)
        SET(LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN/../lib'")

        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-rpath-link,${QT_LIBRARY_DIR} ${LINK_RPATH_FLAG}")
    ENDIF()

    # Try to build our runpath2rpath program, if we are on Linux

    IF(NOT WIN32 AND NOT APPLE)
        SET(RUNPATH2RPATH ${PROJECT_BUILD_DIR}/runpath2rpath)

        EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -o ${RUNPATH2RPATH} ${PROJECT_SOURCE_DIR}/cmake/runpath2rpath.c
                        RESULT_VARIABLE RESULT)

        IF(NOT RESULT EQUAL 0)
            MESSAGE(FATAL_ERROR "runpath2rpath could not be built...")
        ENDIF()
    ENDIF()

    # Let the ExternalProject module know where we want to build build our
    # external projects
    # Note: indeed, otherwise on Windows we may end up with path names that are
    #       too long...

    SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/ext)

    # Show the build information

    MESSAGE("${BUILD_INFORMATION} using Qt ${QT_VERSION}...")
ENDMACRO()

#===============================================================================

MACRO(KEEP_TRACK_OF_FILE FILE_NAME)
    # Keep track of the given file
    # Note: indeed, some files (e.g. versiondate.txt) are 'manually' generated
    #       and then used to build other files. Now, the 'problem' is that
    #       Ninja needs to know about those files (see CMake policy CMP0058),
    #       which we ensure it does through the below...

    ADD_CUSTOM_COMMAND(OUTPUT ${FILE_NAME}
                       COMMAND ${CMAKE_COMMAND} -E sleep 0)
ENDMACRO()

#===============================================================================

MACRO(UPDATE_LANGUAGE_FILES TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files, which will later on be embedded in the project

    SET(LANGUAGES fr)
    SET(INPUT_FILES)

    FOREACH(INPUT_FILE ${ARGN})
        LIST(APPEND INPUT_FILES ${INPUT_FILE})
    ENDFOREACH()

    FOREACH(LANGUAGE ${LANGUAGES})
        SET(LANGUAGE_FILE ${TARGET_NAME}_${LANGUAGE})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)
        SET(QM_FILE ${PROJECT_BUILD_DIR}/${LANGUAGE_FILE}.qm)

        IF(EXISTS ${PROJECT_SOURCE_DIR}/${TS_FILE})
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lupdate -no-obsolete ${INPUT_FILES}
                                                             -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                            OUTPUT_QUIET)
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lrelease ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${QM_FILE}
                            OUTPUT_QUIET)

            KEEP_TRACK_OF_FILE(${QM_FILE})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(OPTIONS
        THIRD_PARTY
    )
    SET(ONE_VALUE_KEYWORDS
        EXTERNAL_BINARIES_DIR
        EXTERNAL_DEST_DIR
        EXTERNAL_SOURCE_DIR
    )
    SET(MULTI_VALUE_KEYWORDS
        SOURCES
        HEADERS_MOC
        UIS
        INCLUDE_DIRS
        DEFINITIONS
        PLUGINS
        PLUGIN_BINARIES
        QT_MODULES
        EXTERNAL_BINARIES
        EXTERNAL_BINARIES_DEPENDENCIES
        SYSTEM_BINARIES
        DEPENDS_ON
        TESTS
    )

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Check whether this is a third-party plugin

    IF(ARG_THIRD_PARTY)
        # Disable all C/C++ warnings since building a third-party plugin may
        # generate some and this has nothing to do with us
        # Note: on Windows, we can't simply add /w since it will otherwise
        #       result in MSVC complaining about /W3 having been overridden by
        #       /w...

        IF(WIN32)
            STRING(REPLACE "/W3" "/w"
                   CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
            STRING(REPLACE "/W3" "/w"
                   CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE()
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
        ENDIF()

        # Add a definition in case of compilation from within Qt Creator using
        # MSVC and JOM since the latter overrides some of our settings

        IF(WIN32)
            ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
        ENDIF()

        # Prevent all debug outputs

        ADD_DEFINITIONS(-DQT_NO_DEBUG_OUTPUT)
    ENDIF()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${ARG_INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${ARG_INCLUDE_DIRS})

    # Resource files, if any
    # Note: ideally, we would have our resource files named i18n.qrc.in and
    #       ui.qrc for all our plugins, but this is causing problems on Linux
    #       with Qt apparently accepting only one file called i18n.qrc.in or
    #       ui.qrc. The end result is that, on a development machine, plugin
    #       resources (e.g. icons, translations) are not available while,
    #       strangely enough, they are on a deployment machine...

    SET(RESOURCES)
    SET(I18N_QRC_IN_FILENAME ${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_i18n.qrc.in)
    SET(UI_QRC_FILENAME ${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_ui.qrc)

    IF(EXISTS ${I18N_QRC_IN_FILENAME})
        STRING(REPLACE "${CMAKE_SOURCE_DIR}" "${PROJECT_BUILD_DIR}"
               I18N_QRC_FILENAME "${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_i18n.qrc")

        CONFIGURE_FILE(${I18N_QRC_IN_FILENAME}
                       ${I18N_QRC_FILENAME})

        LIST(APPEND RESOURCES ${I18N_QRC_FILENAME})

        # Update the translation (.ts) files and generate the language (.qm)
        # files, which will later on be embedded in the plugin

        UPDATE_LANGUAGE_FILES(${PLUGIN_NAME} ${ARG_SOURCES} ${ARG_HEADERS_MOC} ${ARG_UIS})
    ENDIF()

    IF(EXISTS ${UI_QRC_FILENAME})
        LIST(APPEND RESOURCES ${UI_QRC_FILENAME})
    ENDIF()

    # Definition to make sure that the plugin can be used by other plugins

    ADD_DEFINITIONS(-D${PLUGIN_NAME}_PLUGIN)

    # Some plugin-specific definitions

    FOREACH(ARG_DEFINITION ${ARG_DEFINITIONS})
        ADD_DEFINITIONS(-D${ARG_DEFINITION})
    ENDFOREACH()

    # On Linux, set the RPATH value to use by the plugin

    IF(NOT WIN32 AND NOT APPLE)
        SET(PLUGIN_LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/../../lib'")

        STRING(REPLACE "${LINK_RPATH_FLAG}" "${PLUGIN_LINK_RPATH_FLAG}"
               LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
    ENDIF()

    # Generate and add the different files needed by the plugin

    IF("${ARG_HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT5_WRAP_CPP(SOURCES_MOC ${ARG_HEADERS_MOC})
    ENDIF()

    IF("${ARG_UIS}" STREQUAL "")
        SET(SOURCES_UIS)
    ELSE()
        QT5_WRAP_UI(SOURCES_UIS ${ARG_UIS})
    ENDIF()

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
    ENDIF()

    ADD_LIBRARY(${PROJECT_NAME} SHARED
        ${ARG_SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # OpenCOR plugins

    FOREACH(ARG_PLUGIN ${ARG_PLUGINS})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${ARG_PLUGIN}Plugin
        )
    ENDFOREACH()

    # OpenCOR binaries

    FOREACH(ARG_PLUGIN_BINARY ${ARG_PLUGIN_BINARIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${ARG_PLUGIN_BINARY}
        )
    ENDFOREACH()

    # Qt modules

    FOREACH(ARG_QT_MODULE ${ARG_QT_MODULES})
        FIND_PACKAGE(Qt5${ARG_QT_MODULE} REQUIRED)

        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            Qt5::${ARG_QT_MODULE}
        )
    ENDFOREACH()

    # External binaries

    IF(NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL "")
        # Create a custom target for copying binaries
        # Note: this is to prevent Ninja from getting confused with circular
        #       references...

        SET(COPY_EXTERNAL_BINARIES_TARGET "COPY_${PROJECT_NAME}_EXTERNAL_BINARIES")

        ADD_CUSTOM_TARGET(${COPY_EXTERNAL_BINARIES_TARGET})
        ADD_DEPENDENCIES(${PROJECT_NAME} ${COPY_EXTERNAL_BINARIES_TARGET})

        IF(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
            ADD_DEPENDENCIES(${COPY_EXTERNAL_BINARIES_TARGET} ${ARG_DEPENDS_ON})
        ENDIF()

        FOREACH(ARG_EXTERNAL_BINARY ${ARG_EXTERNAL_BINARIES})
            # Make sure that the external binary exists

            SET(FULL_EXTERNAL_BINARY "${ARG_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}")

            # Only do a direct copy if the file exists and the plugin doesn't
            # depend on any targets

            IF(    EXISTS ${FULL_EXTERNAL_BINARY}
               AND "${ARG_DEPENDS_ON}" STREQUAL "")
                SET(COPY_TARGET DIRECT)
            ELSE()
                SET(COPY_TARGET ${COPY_EXTERNAL_BINARIES_TARGET})
            ENDIF()

            # Copy the external binary to its destination directory, so we can
            # test things without first having to deploy OpenCOR
            # Note: on Windows, we also need to copy it to the build directory
            #       so that we can test things from within Qt Creator...

            IF(WIN32)
                COPY_FILE_TO_BUILD_DIR(${COPY_TARGET} ${ARG_EXTERNAL_BINARIES_DIR} . ${ARG_EXTERNAL_BINARY})
            ENDIF()

            COPY_FILE_TO_BUILD_DIR(${COPY_TARGET} ${ARG_EXTERNAL_BINARIES_DIR} ${DEST_EXTERNAL_BINARIES_DIR} ${ARG_EXTERNAL_BINARY})

            # Strip the external library of all its local symbols, if possible

            IF(NOT WIN32 AND RELEASE_MODE)
                IF(${COPY_TARGET} STREQUAL "DIRECT")
                    EXECUTE_PROCESS(COMMAND strip -x ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                ELSE()
                    ADD_CUSTOM_COMMAND(TARGET ${COPY_EXTERNAL_BINARIES_TARGET} POST_BUILD
                                       COMMAND strip -x ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                ENDIF()
            ENDIF()

            # Link the plugin to the external library

            IF(WIN32)
                STRING(REGEX REPLACE "${CMAKE_SHARED_LIBRARY_SUFFIX}$" "${CMAKE_IMPORT_LIBRARY_SUFFIX}"
                       IMPORT_EXTERNAL_BINARY "${FULL_EXTERNAL_BINARY}")

                TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                    ${IMPORT_EXTERNAL_BINARY}
                )
            ELSE()
                TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                    ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}
                )
            ENDIF()

            # On macOS, ensure that @rpath is set in the external library's id
            # and that it is used to reference the external library's
            # dependencies

            IF(APPLE)
                IF(${COPY_TARGET} STREQUAL "DIRECT")
                    EXECUTE_PROCESS(COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                ELSE()
                    ADD_CUSTOM_COMMAND(TARGET ${COPY_EXTERNAL_BINARIES_TARGET} POST_BUILD
                                       COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                ENDIF()

                FOREACH(EXTERNAL_BINARIES_DEPENDENCY ${EXTERNAL_BINARIES_DEPENDENCIES})
                    IF(${COPY_TARGET} STREQUAL "DIRECT")
                        EXECUTE_PROCESS(COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DEPENDENCY}
                                                                          @rpath/${EXTERNAL_BINARIES_DEPENDENCY}
                                                                          ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                    ELSE()
                        ADD_CUSTOM_COMMAND(TARGET ${COPY_EXTERNAL_BINARIES_TARGET} POST_BUILD
                                           COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DEPENDENCY}
                                                                             @rpath/${EXTERNAL_BINARIES_DEPENDENCY}
                                                                             ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                    ENDIF()
                ENDFOREACH()
            ENDIF()

            # Package the external library, if needed

            IF(WIN32)
                INSTALL(FILES ${FULL_EXTERNAL_BINARY}
                        DESTINATION bin)
            ELSEIF(NOT APPLE)
                INSTALL(FILES ${FULL_EXTERNAL_BINARY}
                        DESTINATION lib)
            ENDIF()
        ENDFOREACH()
    ENDIF()

    # Check whether an external package has files to install

    IF(    NOT "${ARG_EXTERNAL_DEST_DIR}" STREQUAL ""
       AND NOT "${ARG_EXTERNAL_SOURCE_DIR}" STREQUAL "")
        # Copy the entire source directory to the destination

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_EXTERNAL_SOURCE_DIR}
                                                                      ${FULL_DEST_EXTERNAL_BASE_DIR}/${ARG_EXTERNAL_DEST_DIR})
    ENDIF()

    # System binaries

    FOREACH(ARG_SYSTEM_BINARY ${ARG_SYSTEM_BINARIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${ARG_SYSTEM_BINARY}
        )
    ENDFOREACH()

    # What must be built before the plugin is built

    IF(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
        ADD_DEPENDENCIES(${PROJECT_NAME} ${ARG_DEPENDS_ON})
    ENDIF()

    # Some settings

    IF(XCODE)
        SET(PLUGIN_BUILD_DIR ${PROJECT_BUILD_DIR})
    ELSE()
        STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
               PLUGIN_BUILD_DIR "${PROJECT_SOURCE_DIR}")

        SET(PLUGIN_BUILD_DIR ${CMAKE_BINARY_DIR}/${PLUGIN_BUILD_DIR})

        IF(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
            SET(PLUGIN_BUILD_DIR ${PLUGIN_BUILD_DIR}/${CMAKE_CFG_INTDIR})
        ENDIF()
    ENDIF()

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR, as well
    #       as, on Windows, test things from within Qt Creator...

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                        ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # A few macOS specific things

    IF(APPLE)
        # Clean up our plugin

        MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${PROJECT_NAME} ${DEST_PLUGINS_DIR} ${PLUGIN_FILENAME})
    ENDIF()

    # Package the plugin, but only if we are not on macOS since it will have
    # already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${CMAKE_PROJECT_NAME})
    ENDIF()

    # Create some tests, if any and if required

    IF(ENABLE_TESTS)
        FOREACH(ARG_TEST ${ARG_TESTS})
            # Keep track of the test (for later use by our main test program)

            FILE(APPEND ${TESTS_LIST_FILENAME} "${PLUGIN_NAME}|${ARG_TEST}|")

            # Build our test, if possible

            SET(TEST_NAME ${PLUGIN_NAME}_${ARG_TEST})

            SET(TEST_SOURCE tests/${ARG_TEST}.cpp)
            SET(TEST_HEADER_MOC tests/${ARG_TEST}.h)

            IF(    EXISTS ${PROJECT_SOURCE_DIR}/${TEST_SOURCE}
               AND EXISTS ${PROJECT_SOURCE_DIR}/${TEST_HEADER_MOC})
                # The test exists, so build it, but first set the RPATH and
                # RPATH link values to use by the test, if on Linux

                IF(NOT WIN32 AND NOT APPLE)
                    STRING(REPLACE "${PLUGIN_LINK_RPATH_FLAG}" "-Wl,-rpath-link,${PROJECT_BUILD_DIR}/lib ${LINK_RPATH_FLAG} -Wl,-rpath,'$ORIGIN/../plugins/${CMAKE_PROJECT_NAME}'"
                           LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
                ENDIF()


                SET(TEST_SOURCES_MOC)
                # Note: we need to initialise TEST_SOURCES_MOC in case there is
                #       more than just one test. Indeed, if we were not to do
                #       that initialisation, then TEST_SOURCES_MOC would include
                #       the information of all the tests up to the one we want
                #       build...

                QT5_WRAP_CPP(TEST_SOURCES_MOC ${TEST_HEADER_MOC})
                QT5_ADD_RESOURCES(TEST_SOURCES_RCS ${TESTS_QRC_FILENAME})

                ADD_EXECUTABLE(${TEST_NAME}
                    ../../../tests/src/testsutils.cpp

                    ${ARG_SOURCES}
                    ${SOURCES_MOC}
                    ${SOURCES_UIS}
                    ${SOURCES_RCS}

                    ${TEST_SOURCE}
                    ${TEST_SOURCES_MOC}
                    ${TEST_SOURCES_RCS}
                )

                SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES
                    OUTPUT_NAME ${TEST_NAME}
                    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
                )

                # Plugins

                FOREACH(ARG_PLUGIN ${ARG_PLUGINS})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${ARG_PLUGIN}Plugin
                    )
                ENDFOREACH()

                # OpenCOR binaries

                FOREACH(ARG_PLUGIN_BINARY ${ARG_PLUGIN_BINARIES})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${ARG_PLUGIN_BINARY}
                    )
                ENDFOREACH()

                # Qt modules

                FOREACH(ARG_QT_MODULE ${ARG_QT_MODULES} Test)
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        Qt5::${ARG_QT_MODULE}
                    )
                ENDFOREACH()

                # External binaries

                IF(NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL "")
                    FOREACH(ARG_EXTERNAL_BINARY ${ARG_EXTERNAL_BINARIES})
                        IF(WIN32)
                            STRING(REGEX REPLACE "${CMAKE_SHARED_LIBRARY_SUFFIX}$" "${CMAKE_IMPORT_LIBRARY_SUFFIX}"
                                   IMPORT_EXTERNAL_BINARY "${ARG_EXTERNAL_BINARY}")

                            TARGET_LINK_LIBRARIES(${TEST_NAME}
                                ${ARG_EXTERNAL_BINARIES_DIR}/${IMPORT_EXTERNAL_BINARY}
                            )
                        ELSE()
                            TARGET_LINK_LIBRARIES(${TEST_NAME}
                                ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}
                            )
                        ENDIF()
                    ENDFOREACH()
                ENDIF()

                # System binaries

                FOREACH(ARG_SYSTEM_BINARY ${ARG_SYSTEM_BINARIES})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${ARG_SYSTEM_BINARY}
                    )
                ENDFOREACH()

                # Copy the test to our tests directory
                # Note: DEST_TESTS_DIR is defined in our main CMake file...

                SET(TEST_FILENAME ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})

                IF(WIN32)
                    ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${TEST_FILENAME}
                                                                        ${PROJECT_BUILD_DIR}/${TEST_FILENAME})
                ENDIF()

                ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${TEST_FILENAME}
                                                                    ${DEST_TESTS_DIR}/${TEST_FILENAME})

                # A few macOS specific things

                IF(APPLE)
                    # Clean up our plugin's tests

                    MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${TEST_NAME} ${DEST_TESTS_DIR} ${TEST_FILENAME})
                ENDIF()
            ELSE()
                MESSAGE(AUTHOR_WARNING "The '${ARG_TEST}' test for the '${PLUGIN_NAME}' plugin does not exist...")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(ADD_PLUGIN_BINARY PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(OPTIONS)
    SET(ONE_VALUE_KEYWORDS)
    SET(MULTI_VALUE_KEYWORDS
        INCLUDE_DIRS
    )

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${ARG_INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${ARG_INCLUDE_DIRS})

    # Some settings

    SET(PLUGIN_BINARY_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})
    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package and deploy
    #       everything...

    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                                                     ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # On macOS, ensure that @rpath is set in the plugin binary's id

    IF(APPLE)
        EXECUTE_PROCESS(COMMAND install_name_tool -id @rpath/${PLUGIN_FILENAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})
    ENDIF()

    # Package the plugin, but only if we are not on macOS since it will have
    # already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${CMAKE_PROJECT_NAME})
    ENDIF()

    # On macOS, and in case we are on Travis CI, make sure that the plugin
    # binary refers to the system version of the Qt libraries since we don't
    # embed the Qt libraries in that case (see the main CMakeLists.txt file)

    IF(APPLE AND ENABLE_TRAVIS_CI)
        FOREACH(MACOS_QT_LIBRARY ${MACOS_QT_LIBRARIES})
            SET(MACOS_QT_LIBRARY_FILENAME ${MACOS_QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${MACOS_QT_LIBRARY})

            EXECUTE_PROCESS(COMMAND install_name_tool -change @rpath/${MACOS_QT_LIBRARY_FILENAME}
                                                              ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME}
                                                              ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})
        ENDFOREACH()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_CONFIG_FILES)
    FOREACH(CONFIG_FILE ${ARGN})
        STRING(REPLACE "PLATFORM_DIR/" "${PLATFORM_DIR}/"
               CONFIG_FILE_ORIG "${CONFIG_FILE}")
        STRING(REPLACE "PLATFORM_DIR/" ""
               CONFIG_FILE_DEST "${CONFIG_FILE}")

        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_ORIG}
                                                         ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_DEST})
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(COPY_FILE_TO_BUILD_DIR PROJECT_TARGET ORIG_DIRNAME DEST_DIRNAME FILENAME)
    # Copy the file (renaming it, if needed) to the destination folder
    # Note: DIRECT is used to copy a file that doesn't first need to be built.
    #       This means that we can then use EXECUTE_PROCESS() rather than
    #       ADD_CUSTOM_COMMAND(), and thus reduce the length of the
    #       PROJECT_TARGET command, something that can be useful on Windows
    #       since the command might otherwise end up being too long for Windows
    #       to handle...

    IF("${ARGN}" STREQUAL "")
        SET(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIRNAME}/${FILENAME})
    ELSE()
        # An argument was passed so use it to rename the file, which is to be
        # copied

        SET(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIRNAME}/${ARGN})
    ENDIF()

    IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                         ${FULL_DEST_FILENAME})
    ELSE()
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${FULL_DEST_FILENAME}
                           BYPRODUCTS ${FULL_DEST_FILENAME})
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(WINDOWS_DEPLOY_QT_LIBRARY LIBRARY_NAME)
    # Copy the Qt library to both the build and build/bin folders, so we can
    # test things both from within Qt Creator and without first having to deploy
    # OpenCOR

    IF(   "${LIBRARY_NAME}" STREQUAL "Qt5WebKit"
       OR "${LIBRARY_NAME}" STREQUAL "Qt5WebKitWidgets"
       OR "${LIBRARY_NAME}" STREQUAL "icudt57"
       OR "${LIBRARY_NAME}" STREQUAL "icuin57"
       OR "${LIBRARY_NAME}" STREQUAL "icuuc57")
        SET(REAL_QT_BINARY_DIR ${QT_WEBKIT_BINARIES_DIR})
    ELSE()
        SET(REAL_QT_BINARY_DIR ${QT_BINARY_DIR})
    ENDIF()

    SET(LIBRARY_RELEASE_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(LIBRARY_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX})

    IF(NOT EXISTS ${REAL_QT_BINARY_DIR}/${LIBRARY_DEBUG_FILENAME})
        # No debug version of the Qt library exists, so use its release version
        # instead

        SET(LIBRARY_DEBUG_FILENAME ${LIBRARY_RELEASE_FILENAME})
    ENDIF()

    IF(RELEASE_MODE)
        SET(LIBRARY_FILENAME ${LIBRARY_RELEASE_FILENAME})
    ELSE()
        SET(LIBRARY_FILENAME ${LIBRARY_DEBUG_FILENAME})
    ENDIF()

    COPY_FILE_TO_BUILD_DIR(DIRECT ${REAL_QT_BINARY_DIR} . ${LIBRARY_FILENAME})
    COPY_FILE_TO_BUILD_DIR(DIRECT ${REAL_QT_BINARY_DIR} bin ${LIBRARY_FILENAME})

    # Deploy the Qt library

    INSTALL(FILES ${REAL_QT_BINARY_DIR}/${LIBRARY_FILENAME}
            DESTINATION bin)
ENDMACRO()

#===============================================================================

MACRO(WINDOWS_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        SET(PLUGIN_ORIG_DIRNAME ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        SET(PLUGIN_DEST_DIRNAME plugins/${PLUGIN_CATEGORY})
        SET(PLUGIN_RELEASE_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(PLUGIN_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX})

        IF(RELEASE_MODE)
            SET(PLUGIN_FILENAME ${PLUGIN_RELEASE_FILENAME})
        ELSE()
            SET(PLUGIN_FILENAME ${PLUGIN_DEBUG_FILENAME})
        ENDIF()

        COPY_FILE_TO_BUILD_DIR(DIRECT ${PLUGIN_ORIG_DIRNAME} ${PLUGIN_DEST_DIRNAME} ${PLUGIN_FILENAME})

        # Deploy the Qt plugin

        INSTALL(FILES ${PLUGIN_ORIG_DIRNAME}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIRNAME})
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(RUNPATH2RPATH FILENAME)
    # Convert the RUNPATH value, if any, of the given ELF file to an RPATH value

    EXECUTE_PROCESS(COMMAND ${RUNPATH2RPATH} ${FILENAME}
                    RESULT_VARIABLE RESULT)

    IF(NOT RESULT EQUAL 0)
        MESSAGE(FATAL_ERROR "The RUNPATH value of '${FILENAME}' could not be converted to a RPATH value...")
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(LINUX_DEPLOY_QT_LIBRARY DIRNAME FILENAME)
    # Copy the Qt library to the build/lib folder, so we can test things without
    # first having to deploy OpenCOR
    # Note: this is particularly useful when the Linux machine has different
    #       versions of Qt...

    COPY_FILE_TO_BUILD_DIR(DIRECT ${DIRNAME} lib ${FILENAME})

    # Make sure that the RUNPATH value is converted to an RPATH value

    RUNPATH2RPATH(lib/${FILENAME})

    # Strip the Qt library of all its local symbols

    IF(RELEASE_MODE)
        EXECUTE_PROCESS(COMMAND strip -x lib/${FILENAME})
    ENDIF()

    # Deploy the Qt library

    INSTALL(FILES ${PROJECT_BUILD_DIR}/lib/${FILENAME}
            DESTINATION lib)
ENDMACRO()

#===============================================================================

MACRO(LINUX_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        SET(PLUGIN_ORIG_DIRNAME ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        SET(PLUGIN_DEST_DIRNAME plugins/${PLUGIN_CATEGORY})
        SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        COPY_FILE_TO_BUILD_DIR(DIRECT ${PLUGIN_ORIG_DIRNAME} ${PLUGIN_DEST_DIRNAME} ${PLUGIN_FILENAME})

        # Make sure that the RUNPATH value is converted to an RPATH value

        RUNPATH2RPATH(${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME})

        # Strip the Qt plugin of all its local symbols

        IF(RELEASE_MODE)
            EXECUTE_PROCESS(COMMAND strip -x ${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME})
        ENDIF()

        # Deploy the Qt plugin

        INSTALL(FILES ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIRNAME})
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(MACOS_CLEAN_UP_FILE PROJECT_TARGET DIRNAME FILENAME)
    # Strip the file of all its local symbols

    SET(FULL_FILENAME ${DIRNAME}/${FILENAME})

    IF(RELEASE_MODE)
        IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
            EXECUTE_PROCESS(COMMAND strip -x ${FULL_FILENAME})
        ELSE()
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND strip -x ${FULL_FILENAME})
        ENDIF()
    ENDIF()

    # Clean up the file's id

    IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
        EXECUTE_PROCESS(COMMAND install_name_tool -id @rpath/${FILENAME} ${FULL_FILENAME})
    ELSE()
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND install_name_tool -id @rpath/${FILENAME} ${FULL_FILENAME})
    ENDIF()

    # Make sure that the file refers to our embedded copy of OpenSSL
    # Note: we try two different paths for a given OpenSSL library since the
    #       former path will be used by libssl.1.0.0.dylib while the latter path
    #       will be used by our plugins...

    FOREACH(OPENSSL_LIBRARY ${OPENSSL_LIBRARIES})
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY ${OPENSSL_LIBRARY} REALPATH)
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY_DIRNAME ${OPENSSL_LIBRARY} DIRECTORY)
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY_FILENAME ${REAL_OPENSSL_LIBRARY} NAME)

        IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
            EXECUTE_PROCESS(COMMAND install_name_tool -change ${REAL_OPENSSL_LIBRARY}
                                                              @rpath/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                              ${FULL_FILENAME})
            EXECUTE_PROCESS(COMMAND install_name_tool -change ${REAL_OPENSSL_LIBRARY_DIRNAME}/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                              @rpath/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                              ${FULL_FILENAME})
        ELSE()
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND install_name_tool -change ${REAL_OPENSSL_LIBRARY}
                                                                 @rpath/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                                 ${FULL_FILENAME})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND install_name_tool -change ${REAL_OPENSSL_LIBRARY_DIRNAME}/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                                 @rpath/${REAL_OPENSSL_LIBRARY_FILENAME}
                                                                 ${FULL_FILENAME})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES PROJECT_TARGET DIRNAME FILENAME)
    # Clean up the file

    MACOS_CLEAN_UP_FILE(${PROJECT_TARGET} ${DIRNAME} ${FILENAME})

    # Make sure that the file refers to our embedded copy of the Qt libraries,
    # but only if we are not on Travis CI (since we don't embed the Qt libraries
    # in that case; see the main CMakeLists.txt file)

    IF(NOT ENABLE_TRAVIS_CI)
        FOREACH(MACOS_QT_LIBRARY ${MACOS_QT_LIBRARIES})
            SET(MACOS_QT_LIBRARY_FILENAME ${MACOS_QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${MACOS_QT_LIBRARY})

            IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
                EXECUTE_PROCESS(COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME}
                                                                  @rpath/${MACOS_QT_LIBRARY_FILENAME}
                                                                  ${FULL_FILENAME})
            ELSE()
                ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                                   COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME}
                                                                     @rpath/${MACOS_QT_LIBRARY_FILENAME}
                                                                     ${FULL_FILENAME})
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(MACOS_DEPLOY_LIBRARY DIRNAME FILENAME)
    # Copy the library

    SET(DEST_DIRNAME ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)

    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${DIRNAME}/${FILENAME}
                                                     ${DEST_DIRNAME}/${FILENAME})

    # Make sure the library is writable (so we can actually clean it up)

    EXECUTE_PROCESS(COMMAND chmod 755 ${DEST_DIRNAME}/${FILENAME})

    # Clean up the library

    MACOS_CLEAN_UP_FILE(DIRECT ${DEST_DIRNAME} ${FILENAME})
ENDMACRO()

#===============================================================================

MACRO(MACOS_DEPLOY_QT_FILE ORIG_DIRNAME DEST_DIRNAME FILENAME)
    # Copy the Qt file

    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                     ${DEST_DIRNAME}/${FILENAME})

    # Clean up the Qt file

    MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(DIRECT ${DEST_DIRNAME} ${FILENAME})
ENDMACRO()

#===============================================================================

MACRO(MACOS_DEPLOY_QT_LIBRARY LIBRARY_NAME)
    # Deploy the Qt library

    SET(QT_FRAMEWORK_DIR ${LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR})

    IF(   "${LIBRARY_NAME}" STREQUAL "QtWebKit"
       OR "${LIBRARY_NAME}" STREQUAL "QtWebKitWidgets")
        SET(REAL_QT_LIBRARY_DIR ${QT_WEBKIT_LIBRARIES_DIR})
    ELSE()
        SET(REAL_QT_LIBRARY_DIR ${QT_LIBRARY_DIR})
    ENDIF()

    MACOS_DEPLOY_QT_FILE(${REAL_QT_LIBRARY_DIR}/${QT_FRAMEWORK_DIR}
                         ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks/${QT_FRAMEWORK_DIR}
                         ${LIBRARY_NAME})
ENDMACRO()

#===============================================================================

MACRO(MACOS_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin

        MACOS_DEPLOY_QT_FILE(${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}
                             ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${PLUGIN_CATEGORY}
                             ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    ENDFOREACH()
ENDMACRO()

#===============================================================================

MACRO(CREATE_PACKAGE_FILE PACKAGE_NAME PACKAGE_VERSION DIRNAME)
    SET(OPTIONS)
    SET(ONE_VALUE_KEYWORDS
        TARGET
    )
    SET(MULTI_VALUE_KEYWORDS
        PACKAGED_FILES
        SHA1_FILES
    )

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # The full path to the package's files

    SET(REAL_DIRNAME "${PROJECT_SOURCE_DIR}/${DIRNAME}")

    # The package name in uppercase

    STRING(TOUPPER ${PACKAGE_NAME} UPPER_PACKAGE_NAME)

    # The name of the package's archive

    SET(COMPRESSED_FILENAME ${PACKAGE_NAME}.${PACKAGE_VERSION}.tar.gz)
    SET(REAL_COMPRESSED_FILENAME ${REAL_DIRNAME}/${COMPRESSED_FILENAME})

    # Remove any historical package archive

    FILE(REMOVE ${REAL_COMPRESSED_FILENAME})

    # Where we put the CMake code to retrieve the archived package

    SET(RETRIEVAL_SCRIPT "${REAL_DIRNAME}/retrievePackage.cmake")

    # The actual packaging code goes into a separate CMake script file that is
    # run as a POST_BUILD step

    SET(PACKAGING_SCRIPT "${PROJECT_BINARY_DIR}/package_${PACKAGE_NAME}.cmake")

    FILE(WRITE ${PACKAGING_SCRIPT} "CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

# Files and directories to package

SET(PACKAGED_FILES\n")

    FOREACH(FILENAME IN LISTS ARG_PACKAGED_FILES)
        FILE(APPEND ${PACKAGING_SCRIPT} "    ${FILENAME}\n")
    ENDFOREACH()

    FILE(APPEND ${PACKAGING_SCRIPT} ")\n")
    FILE(APPEND ${PACKAGING_SCRIPT} "\n")
    FILE(APPEND ${PACKAGING_SCRIPT} "# Files to have their SHA-1 value checked

SET(SHA1_FILES\n")

    FOREACH(FILENAME IN LISTS ARG_SHA1_FILES)
        FILE(APPEND ${PACKAGING_SCRIPT} "    ${FILENAME}\n")
    ENDFOREACH()

    FILE(APPEND ${PACKAGING_SCRIPT} ")\n")
    FILE(APPEND ${PACKAGING_SCRIPT} "\n")
    FILE(APPEND ${PACKAGING_SCRIPT} "# Calculate the SHA-1 value of our different files

SET(SHA1_VALUES)

FOREACH(SHA1_FILE IN LISTS SHA1_FILES)
    SET(REAL_SHA1_FILENAME \"${REAL_DIRNAME}/\$\{SHA1_FILE\}\")

    IF(NOT EXISTS \$\{REAL_SHA1_FILENAME\})
        MESSAGE(FATAL_ERROR \"'\$\{REAL_SHA1_FILENAME\}' is missing from the '${PACKAGE_NAME}' package...\")
    ENDIF()

    FILE(SHA1 \$\{REAL_SHA1_FILENAME\} SHA1_VALUE)

    LIST(APPEND SHA1_VALUES \$\{SHA1_VALUE\})
ENDFOREACH()

# Compress our package

EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -czf ${REAL_COMPRESSED_FILENAME} \$\{PACKAGED_FILES\}
                WORKING_DIRECTORY ${REAL_DIRNAME}
                OUTPUT_QUIET)

# Make sure that the compressed version of our package exists

IF(EXISTS ${REAL_COMPRESSED_FILENAME})
    # The compressed version of our package exists, so calculate its SHA-1 value
    # and generate a small CMake file that tells us how we should call the
    # RETRIEVE_PACKAGE_FILE() macro to retrieve that package

    FILE(SHA1 ${REAL_COMPRESSED_FILENAME} SHA1_VALUE)

    STRING(REPLACE \";\" \"\\n                \" SHA1_VALUES \"\$\{SHA1_VALUES\}\")

    FILE(WRITE ${RETRIEVAL_SCRIPT} \"RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} \\$\\{${UPPER_PACKAGE_NAME}_VERSION\\}
    \\$\\{RELATIVE_PROJECT_SOURCE_DIR\\} \$\{SHA1_VALUE\}
    SHA1_FILES \\$\\{SHA1_FILES\\}
    SHA1_VALUES \$\{SHA1_VALUES\}
)
\")
ELSE()
    MESSAGE(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package could not be generated...")
ENDIF()
")

    # Run the packaging script once the dependency target has been satisfied

    ADD_CUSTOM_COMMAND(TARGET ${ARG_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -P ${PACKAGING_SCRIPT}
                       VERBATIM)
ENDMACRO()

#===============================================================================

MACRO(CHECK_FILES DIRECTORY FILE_LIST SHA1_LIST)
    SET(CHECK_FILES_FAILED FALSE)

    # This enables CMake to see the parameters as lists

    set(_LIST ${FILE_LIST})
    set(_SHA1 ${SHA1_LIST})

    LIST(LENGTH _LIST COUNT)
    IF(COUNT)
        MATH(EXPR RANGE "${COUNT} - 1")
        FOREACH(i RANGE ${RANGE})
            LIST(GET _LIST ${i} FILENAME)
            LIST(GET _SHA1 ${i} SHA1_VALUE)

            # Make sure that the file, if it exists,
            # has the expected SHA-1 value

            SET(REAL_FILENAME ${DIRECTORY}/${FILENAME})

            IF(EXISTS ${REAL_FILENAME})
                FILE(SHA1 ${REAL_FILENAME} REAL_SHA1_VALUE)

                IF(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
                    # The file doesn't have the expected SHA-1 value
                    # so remove it and fail the checks

                    FILE(REMOVE ${REAL_FILENAME})
                    SET(CHECK_FILES_FAILED TRUE)
                ENDIF()
            ELSEIF(NOT CHECK_FILES_FAILED)
                # The file is missing so fail the checks

                SET(CHECK_FILES_FAILED TRUE)
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_PACKAGE_FILE_FROM LOCATION PACKAGE_NAME PACKAGE_VERSION DIRNAME
                                 SHA1_VALUE)
    SET(OPTIONS)
    SET(ONE_VALUE_KEYWORDS)
    SET(MULTI_VALUE_KEYWORDS
        SHA1_FILES
        SHA1_VALUES
    )

    # Parse the argument list

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Make sure that we have at least one file for which we want to check the
    # SHA-1 value is that the SHA-1 value(s) are available

    LIST(LENGTH ARG_SHA1_FILES ARG_SHA1_FILES_COUNT)
    LIST(LENGTH ARG_SHA1_VALUES ARG_SHA1_VALUES_COUNT)

    IF(       ARG_SHA1_FILES_COUNT EQUAL 0
       OR NOT ARG_SHA1_FILES_COUNT EQUAL ARG_SHA1_VALUES_COUNT)
        MESSAGE(FATAL_ERROR "At least one file must have its SHA-1 value checked in order to retrieve the '${PACKAGE_NAME}' package...")
    ENDIF()

    # Create our destination folder, if needed

    STRING(REPLACE "${PLATFORM_DIR}" "ext"
           REAL_DIRNAME "${CMAKE_SOURCE_DIR}/${DIRNAME}")

    IF(NOT EXISTS ${REAL_DIRNAME})
        FILE(MAKE_DIRECTORY ${REAL_DIRNAME})
    ENDIF()

    # Check to see if we already have the package's files

    CHECK_FILES("${REAL_DIRNAME}" "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

    IF(CHECK_FILES_FAILED)
        MESSAGE("Retrieving '${PACKAGE_NAME}' into '${REAL_DIRNAME}'...")

        SET(COMPRESSED_FILENAME ${PACKAGE_NAME}.${PACKAGE_VERSION}.tar.gz)
        SET(REAL_COMPRESSED_FILENAME ${REAL_DIRNAME}/${COMPRESSED_FILENAME})

        FILE(DOWNLOAD "${LOCATION}/${DIRNAME}/${COMPRESSED_FILENAME}" ${REAL_COMPRESSED_FILENAME}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the compressed version of the file, should we have managed
        # to retrieve it

        LIST(GET STATUS 0 STATUS_CODE)

        IF(${STATUS_CODE} EQUAL 0)
            CHECK_FILES("${REAL_DIRNAME}" "${COMPRESSED_FILENAME}" "${SHA1_VALUE}")

            IF(CHECK_FILES_FAILED)
                MESSAGE(FATAL_ERROR "'${COMPRESSED_FILENAME}' does not have the expected SHA-1 value...")
            ENDIF()

            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -xzf ${REAL_COMPRESSED_FILENAME}
                            WORKING_DIRECTORY ${REAL_DIRNAME}
                            OUTPUT_QUIET)

            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
        ELSE()
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
            # Note: this is in case we had an HTTP error of sorts, in which case
            #       we would end up with an empty file...

            MESSAGE(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package could not be retrieved...")
        ENDIF()

        # Check that the files, if we managed to retrieve them, have the expected
        # SHA-1 values

        CHECK_FILES("${REAL_DIRNAME}" "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

        IF(CHECK_FILES_FAILED)
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
            MESSAGE(FATAL_ERROR "The files in '${REAL_COMPRESSED_FILENAME}' do not have the expected SHA-1 values...")
        ENDIF()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_PACKAGE_FILE PACKAGE_NAME PACKAGE_VERSION DIRNAME SHA1_VALUE)
    # Retrieve the package off the OpenCOR website

    RETRIEVE_PACKAGE_FILE_FROM("http://www.opencor.ws/packages"
                               ${PACKAGE_NAME} ${PACKAGE_VERSION}
                               ${DIRNAME} ${SHA1_VALUE} ${ARGN})
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_BINARY_FILE_FROM LOCATION DIRNAME FILENAME SHA1_VALUE)
    # Create our destination folder, if needed

    STRING(REPLACE "${PLATFORM_DIR}" "bin"
           REAL_DIRNAME "${CMAKE_SOURCE_DIR}/${DIRNAME}")

    IF(NOT EXISTS ${REAL_DIRNAME})
        FILE(MAKE_DIRECTORY ${REAL_DIRNAME})
    ENDIF()

    # Make sure that the file, if it exists, has the expected SHA-1 value

    SET(REAL_FILENAME ${REAL_DIRNAME}/${FILENAME})

    CHECK_FILES("${REAL_DIRNAME}" "${FILENAME}" "${SHA1_VALUE}")

    # Retrieve the file from the given location, if needed
    # Note: we would normally provide the SHA-1 value to the FILE(DOWNLOAD)
    #       call, but this would create an empty file to start with and if the
    #       file cannot be downloaded for some reason or another, then we would
    #       still have that file and CMake would then complain about its SHA-1
    #       value being wrong (as well as not being able to download the file),
    #       so we handle everything ourselves...

    IF(NOT EXISTS ${REAL_FILENAME})
        # Retrieve the compressed version of the file

        MESSAGE("Retrieving '${DIRNAME}/${FILENAME}'...")

        SET(COMPRESSED_FILENAME ${FILENAME}.tar.gz)
        SET(REAL_COMPRESSED_FILENAME ${REAL_DIRNAME}/${COMPRESSED_FILENAME})

        FILE(DOWNLOAD "${LOCATION}/${DIRNAME}/${COMPRESSED_FILENAME}" ${REAL_COMPRESSED_FILENAME}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the compressed version of the file, should we have managed
        # to retrieve it

        LIST(GET STATUS 0 STATUS_CODE)

        IF(${STATUS_CODE} EQUAL 0)
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -xzf ${REAL_COMPRESSED_FILENAME}
                            WORKING_DIRECTORY ${REAL_DIRNAME}
                            OUTPUT_QUIET)

            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
        ELSE()
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
            # Note: this is in case we had an HTTP error of sorts, in which case
            #       we would end up with an empty file...

            MESSAGE(FATAL_ERROR "The compressed version of '${FILENAME}' could not be retrieved...")
        ENDIF()

        # Check that the file, if we managed to retrieve it, has the expected
        # SHA-1 value

        IF(EXISTS ${REAL_FILENAME})
            CHECK_FILES("${REAL_DIRNAME}" "${FILENAME}" "${SHA1_VALUE}")

            IF(CHECK_FILES_FAILED)
                MESSAGE(FATAL_ERROR "'${FILENAME}' does not have the expected SHA-1 value...")
            ENDIF()
        ELSE()
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})

            MESSAGE(FATAL_ERROR "'${FILENAME}' could not be uncompressed...")
        ENDIF()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_BINARY_FILE DIRNAME FILENAME SHA1_VALUE)
    # Retrieve the binary file off the OpenCOR website

    RETRIEVE_BINARY_FILE_FROM("http://www.opencor.ws/binaries"
                              ${DIRNAME} ${FILENAME} ${SHA1_VALUE})
ENDMACRO()
