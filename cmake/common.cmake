MACRO(INITIALISE_PROJECT)
#    SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Some settings which depend on whether we want a debug or release version
    # of OpenCOR

    IF(WIN32)
        STRING(REPLACE "/W3" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        # Note: MSVC has a /Wall flag, but it results in MSVC being very
        #       pedantic, so instead we use what MSVC recommends for production
        #       code which is /W3 and which is also what CMake uses by
        #       default...

        SET(LINK_FLAGS_PROPERTIES "/STACK:10000000 /MACHINE:X86")
    ELSE()
        SET(CMAKE_CXX_FLAGS "-Wall -Werror")
        SET(LINK_FLAGS_PROPERTIES)
    ENDIF()

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        MESSAGE("Building a debug version...")

        SET(DEBUG_MODE ON)

        # Default compiler settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /DEBUG")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        ENDIF()
    ELSE()
        MESSAGE("Building a release version...")

        SET(DEBUG_MODE OFF)

        # Default compiler and linker settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /MD /O2 /Ob2")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math")
        ENDIF()

        IF(NOT WIN32 AND NOT APPLE)
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
            # Note #1: -Wl,-s strips all the symbols, thus reducing the final
            #          size of OpenCOR or one its shared libraries...
            # Note #2: the above linking option has become obsolete on OS X,
            #          so...
        ENDIF()
    ENDIF()

    # Required packages

    FIND_PACKAGE(Qt5Widgets REQUIRED)

    # Keep track of some information about Qt

    SET(QT_BINARY_DIR ${_qt5_widgets_install_prefix}/bin)
    SET(QT_LIBRARY_DIR ${_qt5_widgets_install_prefix}/lib)
    SET(QT_PLUGINS_DIR ${_qt5_widgets_install_prefix}/plugins)
    SET(QT_VERSION_MAJOR 5)

    # Whether we are building for 32-bit or 64-bit

    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(64BIT_MODE ON)
    ELSE()
        SET(64BIT_MODE OFF)
    ENDIF()

    # Default location of third-party libraries
    # Note: this is only required so that we can quickly test third-party
    #       libraries without first having to package everything...

    IF(NOT WIN32)
        SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
        # Note: MSVC doesn't care about this location, so...
    ENDIF()

    # Location of our plugins so that we don't have to deploy OpenCOR on
    # Windows and Linux before being able to test it

    IF(APPLE)
        SET(DEST_PLUGINS_DIR ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME})
    ELSE()
        SET(DEST_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    # Default location of external dependencies

    IF(WIN32)
        SET(DISTRIB_DIR windows/x86)
    ELSEIF(APPLE)
        SET(DISTRIB_DIR osx)
    ELSE()
        IF(64BIT_MODE)
            SET(DISTRIB_DIR linux/x64)
        ELSE()
            SET(DISTRIB_DIR linux/x86)
        ENDIF()
    ENDIF()

    IF(WIN32)
        IF(DEBUG_MODE)
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/debug)
        ELSE()
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/release)
        ENDIF()
    ELSE()
        SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR})
    ENDIF()

    # Set the RPATH information on Linux
    # Note: this prevent us from having to use the uncool LD_LIBRARY_PATH...

    IF(NOT WIN32 AND NOT APPLE)
        SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/../plugins/${PROJECT_NAME}")
    ENDIF()
ENDMACRO()

MACRO(UPDATE_LANGUAGE_FILES TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files which will later on be embedded in the project
    # itself
    # Note: this requires SOURCES, HEADERS_MOC and UIS to be defined for the
    #       current CMake project, even if that means that these variables are
    #       to be empty (the case with some plugins for example). Indeed, since
    #       otherwise the value of these variables, as defined in a previous
    #       project, may be used, so...

    SET(LANGUAGE_FILES
        ${TARGET_NAME}_fr
    )

    FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

        IF(EXISTS "${PROJECT_SOURCE_DIR}/${TS_FILE}")
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lupdate -no-obsolete
                                                             ${SOURCES} ${HEADERS_MOC} ${UIS}
                                                         -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lrelease ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${CMAKE_BINARY_DIR}/${LANGUAGE_FILE}.qm)
        ENDIF()
    ENDFOREACH()
ENDMACRO()

MACRO(INCLUDE_THIRD_PARTY_LIBRARY MAIN_PROJECT_SOURCE_DIR THIRD_PARTY_LIBRARY)
    SET(MAIN_PROJECT_SOURCE_DIR ${MAIN_PROJECT_SOURCE_DIR})

    INCLUDE(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/${THIRD_PARTY_LIBRARY}/${THIRD_PARTY_LIBRARY}.cmake)
ENDMACRO()

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(SOURCES)
    SET(HEADERS_MOC)
    SET(UIS)
    SET(INCLUDE_DIRS)
    SET(DEFINITIONS)
    SET(OPENCOR_DEPENDENCIES)
    SET(OPENCOR_BINARY_DEPENDENCIES)
    SET(QT_MODULES)
    SET(QT_DEPENDENCIES)
    SET(EXTERNAL_DEPENDENCIES_DIR)
    SET(EXTERNAL_DEPENDENCIES)
    SET(TESTS)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "THIRD_PARTY")
            # We are dealing with a third-party plugin, so disable all warnings
            # since it may generate some and this is not something we can or
            # should have control over
            # Note: for some reasons, MSVC eventually uses /W1, so we can't
            #       replace /W3 /WX with /w since this would conflict with
            #       /W1 and generate a warning, so...

            IF(WIN32)
                STRING(REPLACE "/W3 /WX" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            ELSE()
                STRING(REPLACE "-Wall -Werror" "-w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            ENDIF()

            # Add a definition in case of compilation from within Qt Creator
            # using MSVC since JOM overrides some of our settings, so...

            IF(WIN32)
                ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
            ENDIF()
        ELSEIF(${PARAMETER} STREQUAL "SOURCES")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS_MOC")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "UIS")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 4)
        ELSEIF(${PARAMETER} STREQUAL "DEFINITIONS")
            SET(TYPE_OF_PARAMETER 5)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 6)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_BINARY_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "QT_MODULES")
            SET(TYPE_OF_PARAMETER 8)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 9)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES_DIR")
            SET(TYPE_OF_PARAMETER 10)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 11)
        ELSEIF(${PARAMETER} STREQUAL "TESTS")
            SET(TYPE_OF_PARAMETER 12)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                LIST(APPEND SOURCES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                LIST(APPEND HEADERS_MOC ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                LIST(APPEND UIS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                LIST(APPEND INCLUDE_DIRS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 5)
                LIST(APPEND DEFINITIONS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 6)
                LIST(APPEND OPENCOR_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 7)
                LIST(APPEND OPENCOR_BINARY_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 8)
                LIST(APPEND QT_MODULES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 9)
                LIST(APPEND QT_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 10)
                SET(EXTERNAL_DEPENDENCIES_DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 11)
                LIST(APPEND EXTERNAL_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 12)
                LIST(APPEND TESTS ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Resource file, if any

    SET(QRC_FILE res/${PLUGIN_NAME}.qrc)

    IF(EXISTS "${PROJECT_SOURCE_DIR}/${QRC_FILE}")
        SET(RESOURCES ${QRC_FILE})
    ELSE()
        SET(RESOURCES)
    ENDIF()

    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the plugin itself

    IF(NOT "${RESOURCES}" STREQUAL "")
        UPDATE_LANGUAGE_FILES(${PLUGIN_NAME})
    ENDIF()

    # Definition to make sure that the plugin can be used by other plugins

    ADD_DEFINITIONS(-D${PLUGIN_NAME}_PLUGIN)

    # Some plugin-specific definitions

    FOREACH(DEFINITION ${DEFINITIONS})
        ADD_DEFINITIONS(-D${DEFINITION})
    ENDFOREACH()

    # Generate and add the different files needed by the plugin

    IF("${HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    ENDIF()

    IF("${UIS}" STREQUAL "")
        SET(SOURCES_UIS)
    ELSE()
        QT5_WRAP_UI(SOURCES_UIS ${UIS})
    ENDIF()

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
    ENDIF()

    ADD_LIBRARY(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    # OpenCOR dependencies

    FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${OPENCOR_DEPENDENCY}Plugin
        )
    ENDFOREACH()

    # OpenCOR binary dependencies

    FOREACH(OPENCOR_BINARY_DEPENDENCY ${OPENCOR_BINARY_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${OPENCOR_BINARY_DEPENDENCY}
        )
    ENDFOREACH()

    # Qt modules

    FOREACH(QT_MODULE ${QT_MODULES})
        QT5_USE_MODULES(${PROJECT_NAME}
            ${QT_MODULE}
        )
    ENDFOREACH()

    # Linker settings

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # External dependencies

    IF(NOT ${EXTERNAL_DEPENDENCIES_DIR} STREQUAL "")
        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
            )
        ENDFOREACH()
    ENDIF()

    # Location of our plugins

    IF(WIN32)
        STRING(REPLACE "${${MAIN_PROJECT_NAME}_SOURCE_DIR}" "" PLUGIN_BUILD_DIR ${PROJECT_SOURCE_DIR})
        SET(PLUGIN_BUILD_DIR "${CMAKE_BINARY_DIR}${PLUGIN_BUILD_DIR}")
        # Note: MSVC generate things in a different place to GCC, so...
    ELSE()
        SET(PLUGIN_BUILD_DIR ${LIBRARY_OUTPUT_PATH})
    ENDIF()

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR...

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                        ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # On Windows, make a copy of the plugin to our main build directory, since
    # this is where it will be on Linux and OS X and where any test which
    # requires the plugin will expect it to be, but this is not where MSVC
    # generates the plugin, so...

    IF(WIN32)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                            ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})
    ENDIF()

    # A few OS X specific things

    IF(APPLE)
        # Clean up our plugin

        OS_X_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}
                                                ${PLUGIN_FILENAME} ${QT_DEPENDENCIES})

        # Make sure that the plugin refers to our embedded version of the other
        # plugins on which it depends

        FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${PLUGIN_BUILD_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the
        # binary plugins on which it depends

        FOREACH(OPENCOR_BINARY_DEPENDENCY ${OPENCOR_BINARY_DEPENDENCIES})
            STRING(REPLACE "${PLUGIN_BUILD_DIR}/" "" OPENCOR_BINARY_DEPENDENCY "${OPENCOR_BINARY_DEPENDENCY}")

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${OPENCOR_BINARY_DEPENDENCY}
                                                                 @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${OPENCOR_BINARY_DEPENDENCY}
                                                                 ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the
        # external dependencies on which it depends
        # Note: we do it in two different ways, since some external libraries we
        #       use refer to the library itself (e.g. CellML) while others refer
        #       to some @executable_path information (e.g. LLVM), so...

        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY}
                                                                 ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY}
                                                                 ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself, but only if we are not on OS X since it will
    # have already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    # Create some tests, if any and if required

    IF(ENABLE_TESTING)
        FOREACH(TEST ${TESTS})
            SET(TEST_NAME ${PLUGIN_NAME}_${TEST})

            SET(TEST_SOURCE_FILE test/${TEST}.cpp)
            SET(TEST_HEADER_MOC_FILE test/${TEST}.h)

            IF(    EXISTS ${PROJECT_SOURCE_DIR}/${TEST_SOURCE_FILE}
               AND EXISTS ${PROJECT_SOURCE_DIR}/${TEST_HEADER_MOC_FILE})
                # The test exists, so build it

                # On Linux and OS X, we need to refer to some bits from the Core
                # plugin even if we don't use them, so...

                IF(WIN32)
                    SET(CORE_SOURCES_MOC)
                    SET(CORE_SOURCES)
                ELSE()
                    SET(CORE_SOURCES_MOC
                        ../../misc/Core/src/dockwidget.h
                    )

                    SET(CORE_SOURCES
                        ../../misc/Core/src/commonwidget.cpp
                        ../../misc/Core/src/dockwidget.cpp
                    )
                ENDIF()

                # Rules to build the test

                QT5_WRAP_CPP(TEST_SOURCES_MOC
                    ../../plugin.h
                    ../../pluginmanager.h

                    ${CORE_SOURCES_MOC}

                    ${HEADERS_MOC}
                    ${TEST_HEADER_MOC_FILE}
                )

                ADD_EXECUTABLE(${TEST_NAME}
                    ../../../../test/testutils.cpp

                    ../../coreinterface.cpp
                    ../../interface.cpp
                    ../../plugin.cpp
                    ../../plugininfo.cpp
                    ../../pluginmanager.cpp

                    ${CORE_SOURCES}

                    ${TEST_SOURCE_FILE}
                    ${TEST_SOURCES_MOC}
                )

                # OpenCOR dependencies

                FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES} ${PLUGIN_NAME})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${OPENCOR_DEPENDENCY}Plugin
                    )
                ENDFOREACH()

                # Qt modules

                FOREACH(QT_MODULE ${QT_MODULES} Test)
                    QT5_USE_MODULES(${TEST_NAME}
                        ${QT_MODULE}
                    )
                ENDFOREACH()

                # External dependencies

                IF(NOT ${EXTERNAL_DEPENDENCIES_DIR} STREQUAL "")
                    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
                        TARGET_LINK_LIBRARIES(${TEST_NAME}
                            ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
                        )
                    ENDFOREACH()
                ENDIF()

                # Copy the test to our tests directory
                # Note: DEST_TESTS_DIR is defined in our main CMake file...

                SET(TEST_FILENAME ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})

                ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${TEST_FILENAME}
                                                                    ${DEST_TESTS_DIR}/${TEST_FILENAME})

                # Make sure that, on OS X, the test refers to our test version
                # of the external libraries on which it depends
                # Note: we do it in two different ways, since some external
                #       libraries we use refer to the library itself (e.g.
                #       CellML) while others refer to some @executable_path
                #       information (e.g. LLVM), so...

                IF(APPLE)
                    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                             ${DEST_TESTS_DIR}/${TEST_FILENAME})

                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                             ${DEST_TESTS_DIR}/${TEST_FILENAME})
                    ENDFOREACH()
                ENDIF()
            ELSE()
                MESSAGE(AUTHOR_WARNING "The '${TEST}' test for the '${PLUGIN_NAME}' plugin doesn't exist")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(ADD_PLUGIN_BINARY PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(INCLUDE_DIRS)
    SET(QT_DEPENDENCIES)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 2)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                LIST(APPEND INCLUDE_DIRS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                LIST(APPEND QT_DEPENDENCIES ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Location of our plugins

    SET(PLUGIN_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package and deploy
    #       everything...

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_TARGET(${PLUGIN_NAME}_COPY_PLUGIN_TO_PLUGINS_DIRECTORY ALL
                      COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                                                       ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # Make a copy of the plugin to our main build directory

    ADD_CUSTOM_TARGET(${PLUGIN_NAME}_COPY_PLUGIN_TO_BUILD_DIRECTORY ALL
                      COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                                                       ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})

    # A few OS X specific things

    IF(APPLE)
        # Make sure that the copy of our plugin in our main build directory
        # refers to the system version of the Qt libraries on which it depends
        # Note: indeed, right now, it refers to our embedded version of the Qt
        #       libraries while, if we want the tests to work, it should refer
        #       to the system version of the Qt libraries, so...

        FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
            ADD_CUSTOM_TARGET(${PLUGIN_NAME}_UPDATE_OS_X_QT_REFERENCE ALL
                              COMMAND install_name_tool -change @executable_path/../Frameworks/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                ${LIBRARY_OUTPUT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself, but only if we are not on OS X since it will
    # have already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${MAIN_PROJECT_NAME})
    ENDIF()
ENDMACRO()

MACRO(COPY_FILE_TO_BUILD_DIR ORIG_DIRNAME DEST_DIRNAME FILENAME)
    IF(EXISTS ${CMAKE_BINARY_DIR}/../cmake)
        # A CMake directory exists at the same level as the binary directory,
        # so we are dealing with the main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/${DEST_DIRNAME})
    ELSE()
        # No CMake directory exists at the same level as the binary directory,
        # so we are dealing with a non-main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/../../build/${DEST_DIRNAME})
    ENDIF()

    IF("${ARGN}" STREQUAL "")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${FILENAME})
    ELSE()
        # An argument was passed so use it to rename the file which is to be
        # copied

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${ARGN})
    ENDIF()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_QT_LIBRARIES)
    FOREACH(LIBRARY ${ARGN})
        # Deploy the Qt library itself

        INSTALL(FILES ${QT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_VERSION_MAJOR}${LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin)
    ENDFOREACH()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin itself

        INSTALL(FILES ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION plugins/${PLUGIN_CATEGORY})
    ENDFOREACH()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_LIBRARY DIRNAME FILENAME)
    # Copy the library file to both the build and build/bin folders, so we can
    # test things without first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(${DIRNAME} . ${FILENAME})
    COPY_FILE_TO_BUILD_DIR(${DIRNAME} bin ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME}
            DESTINATION bin)
ENDMACRO()

MACRO(LINUX_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin itself

        INSTALL(FILES ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION plugins/${PLUGIN_CATEGORY})
    ENDFOREACH()
ENDMACRO()

MACRO(LINUX_DEPLOY_LIBRARY DIRNAME FILENAME)
    # Copy the library file to the build folder, so we can test things without
    # first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(${DIRNAME} . ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME} DESTINATION lib)
ENDMACRO()

MACRO(OS_X_QT_DEPENDENCIES FILENAME QT_DEPENDENCIES)
    # Retrieve the file's full-path Qt dependencies as a list

    SET(QT_LIBRARY_DIR_FOR_GREP "\t${QT_LIBRARY_DIR}/")

    EXECUTE_PROCESS(COMMAND otool -L ${FILENAME}
                    COMMAND grep ${QT_LIBRARY_DIR_FOR_GREP}
                    OUTPUT_VARIABLE RAW_QT_DEPENDENCIES)

    STRING(REPLACE "\n" ";" RAW_QT_DEPENDENCIES "${RAW_QT_DEPENDENCIES}")

    # Extract and return the Qt depencies as a list

    SET(${QT_DEPENDENCIES})

    FOREACH(RAW_QT_DEPENDENCY ${RAW_QT_DEPENDENCIES})
        STRING(REPLACE ${QT_LIBRARY_DIR_FOR_GREP} "" RAW_QT_DEPENDENCY "${RAW_QT_DEPENDENCY}")
        STRING(REGEX REPLACE "\\.framework.*$" "" QT_DEPENDENCY "${RAW_QT_DEPENDENCY}")

        LIST(APPEND ${QT_DEPENDENCIES} ${QT_DEPENDENCY})
    ENDFOREACH()
ENDMACRO()

MACRO(OS_X_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES DIRNAME FILENAME)
    # Strip the Qt file of all local symbols

    SET(FULL_FILENAME ${DIRNAME}/${FILENAME})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND strip -x ${FULL_FILENAME})

    # Clean up the Qt file's id

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND install_name_tool -id ${FILENAME}
                                                     ${FULL_FILENAME})

    # Make sure that the Qt file refers to our embedded version of its Qt
    # dependencies

    FOREACH(DEPENDENCY ${ARGN})
        SET(DEPENDENCY_FILENAME ${DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${DEPENDENCY})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${DEPENDENCY_FILENAME}
                                                             @executable_path/../Frameworks/${DEPENDENCY_FILENAME}
                                                             ${FULL_FILENAME})
    ENDFOREACH()
ENDMACRO()

MACRO(OS_X_DEPLOY_QT_FILE ORIG_DIRNAME DEST_DIRNAME FILENAME)
    # Copy the Qt file itself

    SET(ORIG_FILENAME ${ORIG_DIRNAME}/${FILENAME})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_FILENAME}
                                                        ${DEST_DIRNAME}/${FILENAME})

    # Retrieve the Qt file's Qt dependencies

    OS_X_QT_DEPENDENCIES(${ORIG_FILENAME} DEPENDENCIES)

    # Clean up the Qt file

    OS_X_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${DEST_DIRNAME} ${FILENAME} ${DEPENDENCIES})
ENDMACRO()

MACRO(OS_X_DEPLOY_QT_LIBRARIES)
    FOREACH(LIBRARY_NAME ${ARGN})
        # Deploy the Qt library itself

        SET(QT_FRAMEWORK_DIR ${LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR})

        OS_X_DEPLOY_QT_FILE(${QT_LIBRARY_DIR}/${QT_FRAMEWORK_DIR}
                            ${OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks/${QT_FRAMEWORK_DIR}
                            ${LIBRARY_NAME})
    ENDFOREACH()
ENDMACRO()

MACRO(OS_X_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin itself

        OS_X_DEPLOY_QT_FILE(${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}
                            ${OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${PLUGIN_CATEGORY}
                            ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    ENDFOREACH()
ENDMACRO()

MACRO(OS_X_DEPLOY_LIBRARY DIRNAME LIBRARY_NAME)
    # Copy the library itself

    SET(LIBRARY_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(LIBRARY_FILEPATH ${OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks/${LIBRARY_FILENAME})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${DIRNAME}/${LIBRARY_FILENAME}
                                                        ${LIBRARY_FILEPATH})

    # Copy the library to the build directory, so that we can test any plugin
    # that has a dependency on it

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${DIRNAME}/${LIBRARY_FILENAME}
                                                        ${CMAKE_BINARY_DIR}/${LIBRARY_FILENAME})

    # Strip the library of all local symbols

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND strip -x ${LIBRARY_FILEPATH})

    # Make sure that the library refers to our embedded version of the libraries
    # on which it depends

    FOREACH(DEPENDENCY_NAME ${ARGN})
        SET(DEPENDENCY_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${DEPENDENCY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -change ${DEPENDENCY_FILENAME}
                                                             @executable_path/../Frameworks/${DEPENDENCY_FILENAME}
                                                             ${LIBRARY_FILEPATH})
    ENDFOREACH()
ENDMACRO()
