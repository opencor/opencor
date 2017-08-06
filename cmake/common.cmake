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

MACRO(BUILD_PATCHCMAKE)
    # Try to build our patch CMake program

    SET(PATCH ${CMAKE_BINARY_DIR}/patchcmake${CMAKE_EXECUTABLE_SUFFIX})

    IF(NOT EXISTS ${PATH})
        SET(PATCH_SOURCE ${CMAKE_SOURCE_DIR}/cmake/patchcmake.c)

        IF(WIN32)
            EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} ${PATCH_SOURCE} /link /out:${PATCH}
                            RESULT_VARIABLE RESULT)
        ELSE()
            EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -o ${PATCH} ${PATCH_SOURCE}
                            RESULT_VARIABLE RESULT)
        ENDIF()

        IF(NOT RESULT EQUAL 0)
            MESSAGE(FATAL_ERROR "patchcmake could not be built...")
        ENDIF()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(OPTIONS)
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

    # Additional include directories

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
            ELSEIF(APPLE)
                TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                    ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}
                )
            ELSE()
                TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                    ${FULL_EXTERNAL_BINARY}
                )
            ENDIF()

            # On macOS, ensure that @rpath is set in the external library's id,
            # that it is used to reference the external library's dependencies,
            # and that it references the correct Qt libraries, if any at all

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

                MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${COPY_TARGET} ${FULL_DEST_EXTERNAL_BINARIES_DIR} ${ARG_EXTERNAL_BINARY})
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

    FOREACH(ARG_PLUGIN ${ARG_PLUGINS})
        ADD_DEPENDENCIES(${PROJECT_NAME} ${ARG_PLUGIN}Plugin)
    ENDFOREACH()

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

    # Clean up our plugin, if we are on macOS

    IF(APPLE)
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

                # Add the dependency, if any

                IF(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
                    ADD_DEPENDENCIES(${TEST_NAME} ${ARG_DEPENDS_ON})
                ENDIF()

                FOREACH(ARG_PLUGIN ${ARG_PLUGINS})
                    ADD_DEPENDENCIES(${TEST_NAME} ${ARG_PLUGIN}Plugin)
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

                # Clean up our plugin's tests, if we are on macOS

                IF(APPLE)
                    MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${TEST_NAME} ${DEST_TESTS_DIR} ${TEST_FILENAME})
                ENDIF()
            ELSE()
                MESSAGE(AUTHOR_WARNING "The '${ARG_TEST}' test for the '${PLUGIN_NAME}' plugin does not exist...")
            ENDIF()
        ENDFOREACH()
    ENDIF()
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
       OR "${LIBRARY_NAME}" STREQUAL "icudt${ICU_VERSION}"
       OR "${LIBRARY_NAME}" STREQUAL "icuin${ICU_VERSION}"
       OR "${LIBRARY_NAME}" STREQUAL "icuuc${ICU_VERSION}")
        SET(REAL_QT_BINARY_DIR ${QTWEBKIT_BINARIES_DIR})
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
ENDMACRO()

#===============================================================================

MACRO(MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES PROJECT_TARGET DIRNAME FILENAME)
    # Clean up the file

    MACOS_CLEAN_UP_FILE(${PROJECT_TARGET} ${DIRNAME} ${FILENAME})

    # Make sure that the file refers to our embedded copy of the Qt libraries,
    # but only if we are not on Travis CI (since we don't embed the Qt libraries
    # in that case; see the main CMakeLists.txt file)

    FOREACH(MACOS_QT_LIBRARY ${MACOS_QT_LIBRARIES})
        SET(MACOS_QT_LIBRARY_FILENAME ${MACOS_QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${MACOS_QT_LIBRARY})

        IF(ENABLE_TRAVIS_CI)
            SET(OLD_REFERENCE @rpath/${MACOS_QT_LIBRARY_FILENAME})
            SET(NEW_REFERENCE ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME})
        ELSE()
            SET(OLD_REFERENCE ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME})
            SET(NEW_REFERENCE @rpath/${MACOS_QT_LIBRARY_FILENAME})
        ENDIF()

        IF("${PROJECT_TARGET}" STREQUAL "DIRECT")
            EXECUTE_PROCESS(COMMAND install_name_tool -change ${OLD_REFERENCE} ${NEW_REFERENCE} ${FULL_FILENAME})
        ELSE()
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND install_name_tool -change ${OLD_REFERENCE} ${NEW_REFERENCE} ${FULL_FILENAME})
        ENDIF()
    ENDFOREACH()
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

    IF(   "${LIBRARY_NAME}" STREQUAL "${QTWEBKIT}"
       OR "${LIBRARY_NAME}" STREQUAL "${QTWEBKITWIDGETS}")
        SET(REAL_QT_LIBRARY_DIR ${QTWEBKIT_LIBRARIES_DIR})
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
    # Various initialisations

    SET(OPTIONS)
    SET(ONE_VALUE_KEYWORDS
        PACKAGE_REPOSITORY
        RELEASE_TAG
        TARGET_PLATFORM
        TARGET
    )
    SET(MULTI_VALUE_KEYWORDS
        PACKAGED_FILES
        SHA1_FILES
    )

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Make sure that we have at least one file for which we want to check the
    # SHA-1 value

    LIST(LENGTH ARG_SHA1_FILES ARG_SHA1_FILES_COUNT)

    IF(ARG_SHA1_FILES_COUNT EQUAL 0)
        MESSAGE(FATAL_ERROR "At least one file must have its SHA-1 value calculated in order to create the '${PACKAGE_NAME}' package...")
    ENDIF()

    # The full path to the package's files

    SET(REAL_DIRNAME "${PROJECT_SOURCE_DIR}/${DIRNAME}")

    # Remove any historical package archive

    IF(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
        SET(REAL_TARGET_PLATFORM ${ARG_TARGET_PLATFORM})
    ELSE()
        SET(REAL_TARGET_PLATFORM ${TARGET_PLATFORM})
    ENDIF()

    SET(COMPRESSED_FILENAME ${PROJECT_BUILD_DIR}/${PACKAGE_NAME}.${PACKAGE_VERSION}.${REAL_TARGET_PLATFORM}.tar.gz)

    FILE(REMOVE ${COMPRESSED_FILENAME})

    # The actual packaging code goes into a separate CMake script file that is
    # run as a POST_BUILD step

    SET(CMAKE_CODE "CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

# Files and directories to package

SET(PACKAGED_FILES")

    FOREACH(FILENAME IN LISTS ARG_PACKAGED_FILES)
        SET(CMAKE_CODE "${CMAKE_CODE}\n    ${FILENAME}")
    ENDFOREACH()

    SET(CMAKE_CODE "${CMAKE_CODE}\n)

# Files to have their SHA-1 value checked

SET(SHA1_FILES")

    FOREACH(FILENAME IN LISTS ARG_SHA1_FILES)
        SET(CMAKE_CODE "${CMAKE_CODE}\n    ${FILENAME}")
    ENDFOREACH()

    SET(CMAKE_CODE "${CMAKE_CODE}\n)

# Calculate the SHA-1 value of our different files, after having stripped them,
# if needed

SET(SHA1_VALUES)

FOREACH(SHA1_FILE IN LISTS SHA1_FILES)
    SET(REAL_SHA1_FILENAME \"${REAL_DIRNAME}/\$\{SHA1_FILE\}\")

    IF(NOT EXISTS \$\{REAL_SHA1_FILENAME\})
        MESSAGE(FATAL_ERROR \"'\$\{REAL_SHA1_FILENAME\}' is missing from the '${PACKAGE_NAME}' package...\")
    ENDIF()

    IF(NOT WIN32 AND RELEASE_MODE)
        EXECUTE_PROCESS(COMMAND strip -x \$\{REAL_SHA1_FILENAME\})
    ENDIF()

    FILE(SHA1 \$\{REAL_SHA1_FILENAME\} SHA1_VALUE)

    LIST(APPEND SHA1_VALUES \$\{SHA1_VALUE\})
ENDFOREACH()

# Compress our package

EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -czf ${COMPRESSED_FILENAME} \$\{PACKAGED_FILES\}
                WORKING_DIRECTORY ${REAL_DIRNAME}
                OUTPUT_QUIET)

# Make sure that the compressed version of our package exists

IF(EXISTS ${COMPRESSED_FILENAME})
    # The compressed version of our package exists, so calculate its SHA-1 value
    # and let people know how we should call the RETRIEVE_PACKAGE_FILE() macro

    FILE(SHA1 ${COMPRESSED_FILENAME} SHA1_VALUE)

    STRING(REPLACE \"\;\" \"\\n                                  \" SHA1_VALUES \"\$\{SHA1_VALUES\}\")

    MESSAGE(\"To retrieve the '${PACKAGE_NAME}' package, use:
RETRIEVE_PACKAGE_FILE(\\$\\{PACKAGE_NAME\\} \\$\\{PACKAGE_VERSION\\}
                      \\$\\{RELATIVE_PROJECT_SOURCE_DIR\\} \$\{SHA1_VALUE\}")

    IF(NOT "${ARG_PACKAGE_REPOSITORY}" STREQUAL "")
        SET(CMAKE_CODE "${CMAKE_CODE}\n                      PACKAGE_REPOSITORY \\$\\{PACKAGE_REPOSITORY\\}")
    ENDIF()

    IF(NOT "${ARG_RELEASE_TAG}" STREQUAL "")
        SET(CMAKE_CODE "${CMAKE_CODE}\n                      RELEASE_TAG \\$\\{RELEASE_TAG\\}")
    ENDIF()

    IF(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
        SET(CMAKE_CODE "${CMAKE_CODE}\n                      TARGET_PLATFORM \\$\\{TARGET_PLATFORM\\}")
    ENDIF()

    SET(CMAKE_CODE "${CMAKE_CODE}\n                      SHA1_FILES \\$\\{SHA1_FILES\\}
                      SHA1_VALUES \$\{SHA1_VALUES\})\")
ELSE()
    MESSAGE(FATAL_ERROR \"The compressed version of the '${PACKAGE_NAME}' package could not be generated...\")
ENDIF()
")

    SET(PACKAGING_SCRIPT ${PROJECT_BINARY_DIR}/package${PACKAGE_NAME}.cmake)

    FILE(WRITE ${PACKAGING_SCRIPT} ${CMAKE_CODE})

    # Run the packaging script once the dependency target has been satisfied

    ADD_CUSTOM_COMMAND(TARGET ${ARG_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -D RELEASE_MODE=${RELEASE_MODE} -P ${PACKAGING_SCRIPT}
                       VERBATIM)
ENDMACRO()

#===============================================================================

MACRO(CHECK_FILES DIRNAME FILENAMES SHA1_VALUES)
    # By default, everything is OK

    SET(CHECK_FILES_OK TRUE)

    # See our parameters as lists

    SET(FILENAMES_LIST ${FILENAMES})
    SET(SHA1_VALUES_LIST ${SHA1_VALUES})

    # Retrieve the number of SHA-1 files and values we have

    LIST(LENGTH FILENAMES_LIST FILENAMES_COUNT)

    # Make sure that we have some files

    IF(FILENAMES_COUNT)
        # Determine our range

        MATH(EXPR RANGE "${FILENAMES_COUNT}-1")

        # Make sure that our files, if they exist, have the expected SHA-1 value

        FOREACH(I RANGE ${RANGE})
            LIST(GET FILENAMES_LIST ${I} FILENAME)
            LIST(GET SHA1_VALUES_LIST ${I} SHA1_VALUE)

            SET(REAL_FILENAME ${DIRNAME}/${FILENAME})

            IF(EXISTS ${REAL_FILENAME})
                FILE(SHA1 ${REAL_FILENAME} REAL_SHA1_VALUE)

                IF(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
                    # The file doesn't have the expected SHA-1 value, so remove
                    # it and fail the checks

                    FILE(REMOVE ${REAL_FILENAME})

                    SET(CHECK_FILES_OK FALSE)
                ENDIF()
            ELSEIF(CHECK_FILES_OK)
                # The file is missing, so fail the checks

                SET(CHECK_FILES_OK FALSE)
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

#===============================================================================

MACRO(CHECK_FILE DIRNAME FILENAME SHA1_VALUE)
    # Convenience macro

    CHECK_FILES(${DIRNAME} ${FILENAME} ${SHA1_VALUE})

    SET(CHECK_FILE_OK ${CHECK_FILES_OK})
ENDMACRO()

#===============================================================================

MACRO(RETRIEVE_PACKAGE_FILE PACKAGE_NAME PACKAGE_VERSION DIRNAME SHA1_VALUE)
    # Various initialisations

    SET(OPTIONS)
    SET(ONE_VALUE_KEYWORDS
        PACKAGE_REPOSITORY
        RELEASE_TAG
        TARGET_PLATFORM
    )
    SET(MULTI_VALUE_KEYWORDS
        SHA1_FILES
        SHA1_VALUES
    )

    CMAKE_PARSE_ARGUMENTS(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Make sure that we have at least one file for which we need to check the
    # SHA-1 value

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

    # Make sure that we have the expected package's files

    CHECK_FILES(${REAL_DIRNAME} "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

    IF(NOT CHECK_FILES_OK)
        # Something went wrong with the package's files, so retrieve the package

        MESSAGE("Retrieving the '${PACKAGE_NAME}' package...")

        IF(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
            SET(REAL_TARGET_PLATFORM ${ARG_TARGET_PLATFORM})
        ELSE()
            SET(REAL_TARGET_PLATFORM ${TARGET_PLATFORM})
        ENDIF()

        SET(COMPRESSED_FILENAME ${PACKAGE_NAME}.${PACKAGE_VERSION}.${REAL_TARGET_PLATFORM}.tar.gz)
        SET(FULL_COMPRESSED_FILENAME ${REAL_DIRNAME}/${COMPRESSED_FILENAME})

        IF("${ARG_PACKAGE_REPOSITORY}" STREQUAL "")
            STRING(TOLOWER ${PACKAGE_NAME} PACKAGE_REPOSITORY)
        ELSE()
            SET(PACKAGE_REPOSITORY ${ARG_PACKAGE_REPOSITORY})
        ENDIF()

        IF("${ARG_RELEASE_TAG}" STREQUAL "")
            SET(RELEASE_TAG v${PACKAGE_VERSION})
        ELSE()
            SET(RELEASE_TAG ${ARG_RELEASE_TAG})
        ENDIF()

        FILE(DOWNLOAD "https://github.com/opencor/${PACKAGE_REPOSITORY}/releases/download/${RELEASE_TAG}/${COMPRESSED_FILENAME}" ${FULL_COMPRESSED_FILENAME}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the compressed version of the package, should we have
        # managed to retrieve it

        LIST(GET STATUS 0 STATUS_CODE)

        IF(${STATUS_CODE} EQUAL 0)
            CHECK_FILE(${REAL_DIRNAME} ${COMPRESSED_FILENAME} ${SHA1_VALUE})

            IF(NOT CHECK_FILE_OK)
                MESSAGE(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package does not have the expected SHA-1 value...")
            ENDIF()

            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -xzf ${FULL_COMPRESSED_FILENAME}
                            WORKING_DIRECTORY ${REAL_DIRNAME}
                            OUTPUT_QUIET)

            FILE(REMOVE ${FULL_COMPRESSED_FILENAME})
        ELSE()
            FILE(REMOVE ${FULL_COMPRESSED_FILENAME})
            # Note: this is in case we had an HTTP error of sorts, in which case
            #       we would end up with an empty file...

            MESSAGE(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package could not be retrieved...")
        ENDIF()

        # Check that the package's files, if we managed to uncompress the
        # package, have the expected SHA-1 values

        LIST(GET STATUS 0 STATUS_CODE)

        IF(${STATUS_CODE} EQUAL 0)
            CHECK_FILES(${REAL_DIRNAME} "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

            IF(NOT CHECK_FILES_OK)
                MESSAGE(FATAL_ERROR "The files in the '${PACKAGE_NAME}' package do not have the expected SHA-1 values...")
            ENDIF()
        ELSE()
            MESSAGE(FATAL_ERROR "The files in the '${PACKAGE_NAME}' package could not be uncompressed...")
        ENDIF()
    ENDIF()
ENDMACRO()
