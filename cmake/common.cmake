MACRO(INITIALISE_PROJECT)
#    SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Some settings which depend on whether we want a debug or release version
    # of OpenCOR

    SET(LINK_FLAGS_PROPERTIES)

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        MESSAGE("Building a debug version...")

        SET(DEBUG_MODE ON)

        # Default compiler settings

        SET(CMAKE_CXX_FLAGS "-g -O0")
    ELSE()
        SET(CMAKE_BUILD_TYPE "Release")

        MESSAGE("Building a release version...")

        SET(DEBUG_MODE OFF)

        # Default compiler and linker settings

        SET(CMAKE_CXX_FLAGS "-O2 -ffast-math")

        IF(NOT APPLE)
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
            # Note #1: -Wl,-s strips all the symbols, thus reducing the final
            #          size of OpenCOR or one its shared libraries
            # Note #2: the above linking option has become obsolete on Mac OS X,
            #          so...
        ENDIF()
    ENDIF()

    IF(WIN32)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,--enable-auto-import")
        # Note #1: -Wl,--enable-auto-import allows to resolve vtable entries
        #          in DLLs. This is something that we, ideally, wouldn't need
        #          to set, but it happens that this is required for any plugin
        #          that uses LLVM. Indeed, llvm::CallInst needs resolving,
        #          so...
    ENDIF()

    # Required packages

    IF(APPLE)
        # Note: the Qt SDK doesn't, by default, make the Qt binaries available
        #       to the user, hence we must update the user's PATH. However, this
        #       doesn't work on Mac OS X when using Qt Creator, so we must
        #       hard-code qmake's path so that CMake can find Qt from within
        #       Qt Creator...

        SET(QT_QMAKE_EXECUTABLE /Developer/QtSDK/Desktop/Qt/4.8.0/gcc/bin/qmake)
    ENDIF()

    FIND_PACKAGE(Qt4 4.8.0 REQUIRED)

    # Whether we are building for 32-bit or 64-bit

    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(64BIT_MODE ON)
    ELSE()
        SET(64BIT_MODE OFF)
    ENDIF()

    # Default location of third-party libraries
    # Note: this is only required so that we can quickly test third-party
    #       libraries without first having to package everything

    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

    # Default location of external dependencies

    IF(WIN32)
        SET(DISTRIB_DIR windows/x86)
    ELSEIF(APPLE)
        SET(DISTRIB_DIR macosx)
    ELSE()
        IF(64BIT_MODE)
            SET(DISTRIB_DIR linux/x64)
        ELSE()
            SET(DISTRIB_DIR linux/x86)
        ENDIF()
    ENDIF()

    # Set the RPATH information on Linux
    # Note: this prevent us from having to use the uncool LD_LIBRARY_PATH

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
            EXECUTE_PROCESS(COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete
                                                             ${SOURCES} ${HEADERS_MOC} ${UIS}
                                                         -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
            EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${CMAKE_BINARY_DIR}/${LANGUAGE_FILE}.qm)
        ENDIF()
    ENDFOREACH()
ENDMACRO()

MACRO(INCLUDE_THIRD_PARTY_LIBRARIES MAIN_PROJECT_SOURCE_DIR)
    SET(MAIN_PROJECT_SOURCE_DIR ${MAIN_PROJECT_SOURCE_DIR})

    FOREACH(THIRD_PARTY_LIBRARY ${ARGN})
        INCLUDE(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/${THIRD_PARTY_LIBRARY}/${THIRD_PARTY_LIBRARY}.cmake)
    ENDFOREACH()
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
    SET(QT_DEPENDENCIES)
    SET(EXTERNAL_DEPENDENCIES_DIR)
    SET(EXTERNAL_DEPENDENCIES)
    SET(TESTS)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "SOURCES")
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
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES_DIR")
            SET(TYPE_OF_PARAMETER 8)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 9)
        ELSEIF(${PARAMETER} STREQUAL "TESTS")
            SET(TYPE_OF_PARAMETER 10)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                SET(SOURCES ${SOURCES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                SET(HEADERS_MOC ${HEADERS_MOC} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                SET(UIS ${UIS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 5)
                SET(DEFINITIONS ${DEFINITIONS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 6)
                SET(OPENCOR_DEPENDENCIES ${OPENCOR_DEPENDENCIES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 7)
                SET(QT_DEPENDENCIES ${QT_DEPENDENCIES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 8)
                SET(EXTERNAL_DEPENDENCIES_DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 9)
                SET(EXTERNAL_DEPENDENCIES ${EXTERNAL_DEPENDENCIES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 10)
                SET(TESTS ${TESTS} ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Resource file, if any

    SET(QRC_FILE res/${PLUGIN_NAME}.qrc)

    IF(EXISTS "${PROJECT_SOURCE_DIR}/${QRC_FILE}")
        SET(RESOURCES ${QRC_FILE})
    ELSE()
        SET(RESOURCES)
    ENDIF()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the plugin itself

    IF(NOT "${RESOURCES}" STREQUAL "")
        UPDATE_LANGUAGE_FILES(${PLUGIN_NAME})
    ENDIF()

    # Definition to make sure that the plugin can be used by other plugins

    ADD_DEFINITIONS(-D${PLUGIN_NAME}_PLUGIN)

    # Rules to build the plugin

    IF("${HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    ENDIF()

    IF("${UIS}" STREQUAL "")
        SET(SOURCES_UIS)
    ELSE()
        QT4_WRAP_UI(SOURCES_UIS ${UIS})
    ENDIF()

    QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
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

    # Qt dependencies

    FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
        IF(WIN32)
            IF(DEBUG_MODE)
                SET(QT_DEPENDENCY_VERSION d)
            ELSE()
                SET(QT_DEPENDENCY_VERSION)
            ENDIF()

            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${QT_DEPENDENCY}${QT_DEPENDENCY_VERSION}${QT_VERSION_MAJOR}${CMAKE_STATIC_LIBRARY_SUFFIX})
        ELSEIF(APPLE)
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework)
        ELSE()
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDIF()

        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${QT_LIBRARY_PATH}
        )
    ENDFOREACH()

    # External dependencies

    IF(NOT ${EXTERNAL_DEPENDENCIES_DIR} STREQUAL "")
        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
            )
        ENDFOREACH()
    ENDIF()

    # Linker settings
    # Note: by default "lib" will be prepended to the name of the target file.
    #       However, this is not common practice on Windows, so...

    IF(WIN32)
        SET(CMAKE_IMPORT_LIBRARY_PREFIX)
        SET(CMAKE_SHARED_LIBRARY_PREFIX)
    ENDIF()

    SET_TARGET_PROPERTIES(${PROJECT_NAME}
        PROPERTIES OUTPUT_NAME ${PLUGIN_NAME} LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # Create the plugins directory if it doesn't already exist and move the
    # plugin to it
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    IF(APPLE)
        SET(DEST_PLUGINS_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME})
    ELSE()
        SET(DEST_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    IF(NOT EXISTS ${DEST_PLUGINS_DIR})
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_PLUGINS_DIR})
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/${PLUGIN_FILENAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # Add some more definitions

    FOREACH(DEFINITION ${DEFINITIONS})
        ADD_DEFINITIONS(-D${DEFINITION})
    ENDFOREACH()

    # A few Mac OS X specific things

    IF(APPLE)
        # Clean up our plugin's id

        SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -id ${PLUGIN_FILENAME}
                                                         ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

        # Make sure that the plugin refers to our embedded version of the other
        # plugins on which it depends

        FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY_OUTPUT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the Qt
        # libraries on which it depends

        FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the
        # external dependencies on which it depends
        # Note #1: we do it in two different ways, since some external libraries
        #          we use refer to the library itself (e.g. CellML) while others
        #          refer to some @executable_path information (e.g. LLVM), so...
        # Note #2: we must do it for both the deployed and 'test' versions of
        #          the plugin...

        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            # First, for the deployed version of the plugin

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

            # Second, for the 'test' version of the plugin

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                 ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                 ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY}
                                                                 ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                 ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself

    IF(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION plugins/${MAIN_PROJECT_NAME})
    ELSEIF(NOT APPLE)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins/${MAIN_PROJECT_NAME})
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

                QT4_WRAP_CPP(TEST_SOURCES_MOC
                    ${TEST_HEADER_MOC_FILE}
                    ../../plugin.h
                )

                ADD_EXECUTABLE(${TEST_NAME}
                    ${TEST_SOURCE_FILE}
                    ../../../../test/testutils.cpp
                    ../../coreinterface.cpp
                    ../../plugin.cpp
                    ../../plugininfo.cpp
                    ${TEST_SOURCES_MOC}
                )

                # OpenCOR dependencies

                FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${OPENCOR_DEPENDENCY}Plugin
                    )
                ENDFOREACH()

                # Qt dependencies

                FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES} QtTest)
                    IF(WIN32)
                        IF(DEBUG_MODE)
                            SET(QT_DEPENDENCY_VERSION d)
                        ELSE()
                            SET(QT_DEPENDENCY_VERSION)
                        ENDIF()

                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${QT_DEPENDENCY}${QT_DEPENDENCY_VERSION}${QT_VERSION_MAJOR}${CMAKE_STATIC_LIBRARY_SUFFIX})
                    ELSEIF(APPLE)
                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework)
                    ELSE()
                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX})
                    ENDIF()

                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${QT_LIBRARY_PATH}
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

                # Copy the test to our tests directory which we create if
                # needed

                IF(NOT EXISTS ${DEST_TESTS_DIR})
                    ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                       COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_TESTS_DIR})
                ENDIF()

                SET(TEST_NAME_FILEPATH ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})

                ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${TEST_NAME_FILEPATH} ${DEST_TESTS_DIR}/${TEST_NAME_FILEPATH})

                # Make sure that, on Mac OS X, the test refers to our test
                # version of the external libraries on which it depends

                IF(APPLE)
                    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                             ${DEST_TESTS_DIR}/${TEST_NAME_FILEPATH})

                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY}
                                                                             ${DEST_TESTS_DIR}/${TEST_NAME_FILEPATH})
                    ENDFOREACH()
                ENDIF()
            ELSE()
                MESSAGE(AUTHOR_WARNING "The '${TEST}' test for the '${PLUGIN_NAME}' plugin doesn't exist")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(DEPLOY_MAC_OS_X_LIBRARY LIBRARY_NAME)
    # Various initialisations

    SET(TYPE)
    SET(DIR)
    SET(FRAMEWORKS)
    SET(LIBRARIES)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "TYPE")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "DIR")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "FRAMEWORKS")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "LIBRARIES")
            SET(TYPE_OF_PARAMETER 4)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                SET(TYPE ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                SET(DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                SET(FRAMEWORKS ${FRAMEWORKS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                SET(LIBRARIES ${LIBRARIES} ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Create the folder hierarchy where the library/framework is to be deployed

    IF("${TYPE}" STREQUAL "Library")
        SET(LIBRARY_LIB_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks)

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBRARY_LIB_DIR})
    ELSE()
        SET(QT_LIBRARY_HOME_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks/${LIBRARY_NAME}.framework)
        SET(LIBRARY_LIB_DIR ${QT_LIBRARY_HOME_DIR}/Versions/${QT_VERSION_MAJOR})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${QT_LIBRARY_HOME_DIR}/Resources
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBRARY_LIB_DIR})
    ENDIF()

    # Deploy the library itself

    IF("${TYPE}" STREQUAL "Library")
        IF("${DIR}" STREQUAL "")
            # We must deploy a Qt library

            SET(LIBRARY_LIB_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${QT_LIBRARY_DIR}/${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX} ${LIBRARY_LIB_FILEPATH})
        ELSE()
            # We must deploy a third-party library

            SET(LIBRARY_LIB_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${DIR}/${LIBRARY_NAME} ${LIBRARY_LIB_FILEPATH})

            # In the case of a third-party library, we must also copy the
            # library to the build directory, so that we can test any plugin
            # that has a dependency on it

            SET(LIBRARY_LIB_TEST_FILEPATH ${CMAKE_BINARY_DIR}/${LIBRARY_NAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${DIR}/${LIBRARY_NAME} ${LIBRARY_LIB_TEST_FILEPATH})
        ENDIF()
    ELSE()
        # We must deploy a library which is bundled in a Qt framework

        SET(LIBRARY_LIB_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${QT_LIBRARY_DIR}/${LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY_NAME} ${LIBRARY_LIB_FILEPATH})
    ENDIF()

    # Strip the library from anything that is not essential

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND strip -S -x ${LIBRARY_LIB_FILEPATH})

    # Do things that are only related to Qt libraries or non-Qt libraries

    IF("${DIR}" STREQUAL "")
        # Clean up the library's id

        IF("${TYPE}" STREQUAL "Library")
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -id ${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                             ${LIBRARY_LIB_FILEPATH})
        ELSE()
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -id ${LIBRARY_NAME}
                                                             ${LIBRARY_LIB_FILEPATH})
        ENDIF()

        # Make sure that the library refers to our embedded version of the Qt
        # libraries on which it depends

        FOREACH(LIBRARY ${LIBRARIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${LIBRARY}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 @executable_path/../Frameworks/${LIBRARY}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 ${LIBRARY_LIB_FILEPATH})
        ENDFOREACH()

        # Make sure that the library refers to our embedded version of the Qt
        # frameworks on which it depends

        FOREACH(FRAMEWORK ${FRAMEWORKS})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${FRAMEWORK}.framework/Versions/${QT_VERSION_MAJOR}/${FRAMEWORK}
                                                                 @executable_path/../Frameworks/${FRAMEWORK}.framework/Versions/${QT_VERSION_MAJOR}/${FRAMEWORK}
                                                                 ${LIBRARY_LIB_FILEPATH})
        ENDFOREACH()
    ELSE()
        # Make sure that the third-party library refers to our embedded version
        # of the libraries on which it depends
        # Note: we must do it for both the deployed and 'test' versions of the
        #       third-party library...

        FOREACH(LIBRARY ${LIBRARIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}
                                                                 @executable_path/../Frameworks/${LIBRARY}
                                                                 ${LIBRARY_LIB_FILEPATH})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}
                                                                 ${CMAKE_BINARY_DIR}/${LIBRARY}
                                                                 ${LIBRARY_LIB_TEST_FILEPATH})
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT PLUGIN_DIRNAME PLUGIN_NAME)
    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND install_name_tool -id ${PLUGIN_FILENAME}
                                                     ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${PLUGIN_DIRNAME}/${PLUGIN_FILENAME})
ENDMACRO()

MACRO(COPY_FILE_TO_BUILD_DIR DIRNAME FILENAME)
    IF(EXISTS ${CMAKE_BINARY_DIR}/../cmake)
        # A cmake directory exists at the same level as the binary directory,
        # so we are dealing with the main project

        SET(REAL_DIRNAME ${CMAKE_BINARY_DIR}/${DIRNAME})
    ELSE()
        # No cmake directory exists at the same level as the binary directory,
        # so we are dealing with a non-main project

        SET(REAL_DIRNAME ${CMAKE_BINARY_DIR}/../../build/${DIRNAME})
    ENDIF()

    IF(NOT EXISTS ${REAL_DIRNAME})
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${REAL_DIRNAME})
    ENDIF()

    IF("${ARGN}" STREQUAL "")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${FILENAME} ${REAL_DIRNAME})
    ELSE()
        # An argument was passed so use it to rename the file which is to be
        # copied

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${FILENAME} ${REAL_DIRNAME}/${ARGN})
    ENDIF()
ENDMACRO()

MACRO(DEPLOY_WINDOWS_BINARY_FILE FILENAME)
    # Copy the binary file to both the build and build/bin folders, so we can
    # test things without first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(. ${FILENAME})
    COPY_FILE_TO_BUILD_DIR(bin ${FILENAME})

    # Install the binary file

    INSTALL(FILES ${FILENAME} DESTINATION bin)
ENDMACRO()
