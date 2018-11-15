macro(track_files)
    # Keep track of the given files
    # Note: indeed, some files (e.g. versiondate.txt) are 'manually' generated
    #       and then used to build other files. Now, the 'problem' is that
    #       Ninja needs to know about those files (see CMake policy CMP0058),
    #       which we ensure it does through the below...

    foreach(FILENAME ${ARGN})
        add_custom_command(OUTPUT ${FILENAME}
                           COMMAND ${CMAKE_COMMAND} -E sleep 0)
    endforeach()
endmacro()

#===============================================================================

macro(update_language_files TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files, which will later on be embedded in the project

    set(LANGUAGES fr)
    set(INPUT_FILES)

    foreach(INPUT_FILE ${ARGN})
        list(APPEND INPUT_FILES ${INPUT_FILE})
    endforeach()

    foreach(LANGUAGE ${LANGUAGES})
        set(LANGUAGE_FILE ${TARGET_NAME}_${LANGUAGE})
        set(TS_FILE i18n/${LANGUAGE_FILE}.ts)
        set(QM_FILE ${PROJECT_BUILD_DIR}/${LANGUAGE_FILE}.qm)

        if(EXISTS ${PROJECT_SOURCE_DIR}/${TS_FILE})
            execute_process(COMMAND ${QT_BINARY_DIR}/lupdate -no-obsolete ${INPUT_FILES}
                                                             -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                            OUTPUT_QUIET)
            execute_process(COMMAND ${QT_BINARY_DIR}/lrelease ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${QM_FILE}
                            OUTPUT_QUIET)

            track_files(${QM_FILE})
        endif()
    endforeach()
endmacro()

#===============================================================================

macro(build_documentation DOCUMENTATION_NAME)
    # Build the given documentation as an external project and have it copied to
    # our final documentation directory

    set(DOCUMENTATION_BUILD ${DOCUMENTATION_NAME}DocumentationBuild)

    string(REPLACE ";" "|"
           DOCUMENTATION_SPHINX_EXECUTABLE "${SPHINX_EXECUTABLE}")

    ExternalProject_Add(${DOCUMENTATION_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/doc/${DOCUMENTATION_NAME}
        GIT_REPOSITORY
            https://github.com/opencor/${DOCUMENTATION_NAME}-documentation
        CMAKE_ARGS
            -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
            -DSPHINX_EXECUTABLE=${DOCUMENTATION_SPHINX_EXECUTABLE}
        LIST_SEPARATOR
            |
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BUILD_DIR}/ext/Build/${DOCUMENTATION_BUILD}/html
                                               ${PROJECT_BUILD_DIR}/doc/${DOCUMENTATION_NAME}
    )

    # Add our external project as a dependency to our project build target, so
    # that our Help window plugin can generate the help files that will be
    # embedded in OpenCOR as a resource

    add_dependencies(${PROJECT_BUILD_TARGET} ${DOCUMENTATION_BUILD})
endmacro()

#===============================================================================

macro(add_plugin PLUGIN_NAME)
    # Various initialisations

    set(PLUGIN_NAME ${PLUGIN_NAME})

    set(OPTIONS)
    set(ONE_VALUE_KEYWORDS
        EXTERNAL_BINARIES_DIR
        EXTERNAL_DESTINATION_DIR
        EXTERNAL_SOURCE_DIR
    )
    set(MULTI_VALUE_KEYWORDS
        SOURCES
        HEADERS_MOC
        UIS
        DEFINITIONS
        PLUGINS
        QT_MODULES
        EXTERNAL_BINARIES
        SYSTEM_BINARIES
        DEPENDS_ON
        TESTS
    )

    cmake_parse_arguments(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Resource files, if any
    # Note: ideally, we would have our resource files named i18n.qrc.in and
    #       ui.qrc for all our plugins, but this is causing problems on Linux
    #       with Qt apparently accepting only one file called i18n.qrc.in or
    #       ui.qrc. The end result is that, on a development machine, plugin
    #       resources (e.g. icons, translations) are not available while,
    #       strangely enough, they are on a deployment machine...

    set(RESOURCES)
    set(I18N_QRC_IN_FILENAME ${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_i18n.qrc.in)
    set(UI_QRC_FILENAME ${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_ui.qrc)

    if(EXISTS ${I18N_QRC_IN_FILENAME})
        string(REPLACE "${CMAKE_SOURCE_DIR}" "${PROJECT_BUILD_DIR}"
               I18N_QRC_FILENAME "${PROJECT_SOURCE_DIR}/res/${PLUGIN_NAME}_i18n.qrc")

        configure_file(${I18N_QRC_IN_FILENAME}
                       ${I18N_QRC_FILENAME})

        list(APPEND RESOURCES ${I18N_QRC_FILENAME})

        # Update the translation (.ts) files and generate the language (.qm)
        # files, which will later on be embedded in the plugin

        update_language_files(${PLUGIN_NAME} ${ARG_SOURCES} ${ARG_HEADERS_MOC} ${ARG_UIS})
    endif()

    if(EXISTS ${UI_QRC_FILENAME})
        list(APPEND RESOURCES ${UI_QRC_FILENAME})
    endif()

    # Definition to make sure that the plugin can be used by other plugins

    add_definitions(-D${PLUGIN_NAME}_PLUGIN)

    # Some plugin-specific definitions

    foreach(ARG_DEFINITION ${ARG_DEFINITIONS})
        add_definitions(-D${ARG_DEFINITION})
    endforeach()

    # On Linux, set the RPATH value to use by the plugin

    if(NOT WIN32 AND NOT APPLE)
        set(PLUGIN_LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/../../lib'")

        string(REPLACE "${LINK_RPATH_FLAG}" "${PLUGIN_LINK_RPATH_FLAG}"
               LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
    endif()

    # Generate and add the different files needed by the plugin

    if("${ARG_HEADERS_MOC}" STREQUAL "")
        set(SOURCES_MOC)
    else()
        qt5_wrap_cpp(SOURCES_MOC ${ARG_HEADERS_MOC})
    endif()

    if("${ARG_UIS}" STREQUAL "")
        set(SOURCES_UIS)
    else()
        qt5_wrap_ui(SOURCES_UIS ${ARG_UIS})
    endif()

    if("${RESOURCES}" STREQUAL "")
        set(SOURCES_RCS)
    else()
        qt5_add_resources(SOURCES_RCS ${RESOURCES})
    endif()

    add_library(${PROJECT_NAME} SHARED
        ${ARG_SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # OpenCOR plugins

    foreach(ARG_PLUGIN ${ARG_PLUGINS})
        target_link_libraries(${PROJECT_NAME}
            ${ARG_PLUGIN}Plugin
        )
    endforeach()

    # Qt modules

    foreach(ARG_QT_MODULE ${ARG_QT_MODULES})
        find_package(Qt5${ARG_QT_MODULE} REQUIRED)

        target_link_libraries(${PROJECT_NAME}
            Qt5::${ARG_QT_MODULE}
        )
    endforeach()

    # External binaries

    if(NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL "")
        # Create a custom target for copying binaries
        # Note: this is to prevent Ninja from getting confused with circular
        #       references...

        set(COPY_EXTERNAL_BINARIES_TARGET "COPY_${PROJECT_NAME}_EXTERNAL_BINARIES")

        add_custom_target(${COPY_EXTERNAL_BINARIES_TARGET})
        add_dependencies(${PROJECT_NAME} ${COPY_EXTERNAL_BINARIES_TARGET})

        if(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
            add_dependencies(${COPY_EXTERNAL_BINARIES_TARGET} ${ARG_DEPENDS_ON})
        endif()

        foreach(ARG_EXTERNAL_BINARY ${ARG_EXTERNAL_BINARIES})
            # Make sure that the external binary exists

            set(FULL_EXTERNAL_BINARY "${ARG_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}")

            # Only do a direct copy if the file exists and the plugin doesn't
            # depend on any targets

            if(    EXISTS ${FULL_EXTERNAL_BINARY}
               AND "${ARG_DEPENDS_ON}" STREQUAL "")
                set(COPY_TARGET DIRECT)
            else()
                set(COPY_TARGET ${COPY_EXTERNAL_BINARIES_TARGET})
            endif()

            # Copy the external binary to its destination directory, so we can
            # test things without first having to deploy OpenCOR
            # Note: on Windows, we also need to copy it to the build directory
            #       so that we can test things from within Qt Creator...

            if(WIN32)
                copy_file_to_build_dir(${COPY_TARGET} ${ARG_EXTERNAL_BINARIES_DIR} . ${ARG_EXTERNAL_BINARY})
            endif()

            copy_file_to_build_dir(${COPY_TARGET} ${ARG_EXTERNAL_BINARIES_DIR} ${DEST_EXTERNAL_BINARIES_DIR} ${ARG_EXTERNAL_BINARY})

            # Strip the external library of all its local symbols, if possible

            if(NOT WIN32 AND RELEASE_MODE)
                if(${COPY_TARGET} STREQUAL "DIRECT")
                    execute_process(COMMAND strip -x ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                else()
                    add_custom_command(TARGET ${COPY_EXTERNAL_BINARIES_TARGET} POST_BUILD
                                       COMMAND strip -x ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                endif()
            endif()

            # Link the plugin to the external library

            if(WIN32)
                string(REGEX REPLACE "${CMAKE_SHARED_LIBRARY_SUFFIX}$" "${CMAKE_IMPORT_LIBRARY_SUFFIX}"
                       IMPORT_EXTERNAL_BINARY "${FULL_EXTERNAL_BINARY}")

                target_link_libraries(${PROJECT_NAME}
                    ${IMPORT_EXTERNAL_BINARY}
                )
            elseif(APPLE)
                target_link_libraries(${PROJECT_NAME}
                    ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}
                )
            else()
                target_link_libraries(${PROJECT_NAME}
                    ${FULL_EXTERNAL_BINARY}
                )
            endif()

            # On macOS, ensure that @rpath is set in the external library's id
            # and that it references the correct Qt libraries, if any at all

            if(APPLE)
                if(${COPY_TARGET} STREQUAL "DIRECT")
                    execute_process(COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                else()
                    add_custom_command(TARGET ${COPY_EXTERNAL_BINARIES_TARGET} POST_BUILD
                                       COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY})
                endif()

                macos_clean_up_file_with_qt_dependencies(${COPY_TARGET} ${FULL_DEST_EXTERNAL_BINARIES_DIR} ${ARG_EXTERNAL_BINARY})
            endif()

            # Package the external library, if needed

            if(WIN32)
                install(FILES ${FULL_EXTERNAL_BINARY}
                        DESTINATION bin)
            elseif(NOT APPLE)
                install(FILES ${FULL_EXTERNAL_BINARY}
                        DESTINATION lib)
            endif()
        endforeach()
    endif()

    # Check whether an external package has files to install

    if(    NOT "${ARG_EXTERNAL_DESTINATION_DIR}" STREQUAL ""
       AND NOT "${ARG_EXTERNAL_SOURCE_DIR}" STREQUAL "")

        # Copy the entire source directory to the destination

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_EXTERNAL_SOURCE_DIR}
                                                                      ${ARG_EXTERNAL_DESTINATION_DIR})
    endif()

    # System binaries

    foreach(ARG_SYSTEM_BINARY ${ARG_SYSTEM_BINARIES})
        target_link_libraries(${PROJECT_NAME}
            ${ARG_SYSTEM_BINARY}
        )
    endforeach()

    # Add some dependencies, if any

    if(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
        add_dependencies(${PROJECT_NAME} ${ARG_DEPENDS_ON})
    endif()

    foreach(ARG_PLUGIN ${ARG_PLUGINS})
        add_dependencies(${PROJECT_NAME} ${ARG_PLUGIN}Plugin)
    endforeach()

    # Some settings

    if(XCODE)
        set(PLUGIN_BUILD_DIR ${PROJECT_BUILD_DIR})
    else()
        string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
               PLUGIN_BUILD_DIR "${PROJECT_SOURCE_DIR}")

        set(PLUGIN_BUILD_DIR ${PROJECT_BUILD_DIR}/${PLUGIN_BUILD_DIR})

        if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
            set(PLUGIN_BUILD_DIR ${PLUGIN_BUILD_DIR}/${CMAKE_CFG_INTDIR})
        endif()
    endif()

    set(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR, as well
    #       as, on Windows, test things from within Qt Creator...

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                        ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # Clean up our plugin, if we are on macOS, our make sure that it uses RPATH
    # rather than RUNPATH on Linux

    if(APPLE)
        macos_clean_up_file_with_qt_dependencies(${PROJECT_NAME} ${DEST_PLUGINS_DIR} ${PLUGIN_FILENAME})
    elseif(NOT WIN32)
        runpath2rpath(${PROJECT_NAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})
    endif()

    # Package the plugin, but only if we are not on macOS since it will have
    # already been packaged (copied)

    if(NOT APPLE)
        install(FILES ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${CMAKE_PROJECT_NAME})
    endif()

    # Create some tests, if any and if required

    if(ENABLE_TESTS)
        foreach(ARG_TEST ${ARG_TESTS})
            # Keep track of the test (for later use by our main test program)

            file(APPEND ${TESTS_LIST_FILENAME} "${PLUGIN_NAME}|${ARG_TEST}|")

            # Build our test, if possible

            set(TEST_NAME ${PLUGIN_NAME}_${ARG_TEST})

            set(TEST_SOURCE tests/${ARG_TEST}.cpp)
            set(TEST_HEADER_MOC tests/${ARG_TEST}.h)

            if(    EXISTS ${PROJECT_SOURCE_DIR}/${TEST_SOURCE}
               AND EXISTS ${PROJECT_SOURCE_DIR}/${TEST_HEADER_MOC})
                # The test exists, so build it, but first set the RPATH and
                # RPATH link values to use by the test, if on Linux

                if(NOT WIN32 AND NOT APPLE)
                    string(REPLACE "${PLUGIN_LINK_RPATH_FLAG}" "-Wl,-rpath-link,${PROJECT_BUILD_DIR}/lib ${LINK_RPATH_FLAG} -Wl,-rpath,'$ORIGIN/../plugins/${CMAKE_PROJECT_NAME}'"
                           LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
                endif()


                set(TEST_SOURCES_MOC)
                # Note: we need to initialise TEST_SOURCES_MOC in case there is
                #       more than just one test. Indeed, if we were not to
                #       initialise it, then it would include the information of
                #       all the tests up to the one we want to build...

                qt5_wrap_cpp(TEST_SOURCES_MOC ${TEST_HEADER_MOC})
                qt5_add_resources(TEST_SOURCES_RCS ${TESTS_QRC_FILENAME})

                add_executable(${TEST_NAME}
                    ../../../tests/src/testsutils.cpp

                    ${ARG_SOURCES}
                    ${SOURCES_MOC}
                    ${SOURCES_UIS}
                    ${SOURCES_RCS}

                    ${TEST_SOURCE}
                    ${TEST_SOURCES_MOC}
                    ${TEST_SOURCES_RCS}
                )

                set_target_properties(${TEST_NAME} PROPERTIES
                    OUTPUT_NAME ${TEST_NAME}
                    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
                )

                # OpenCOR plugins

                foreach(ARG_PLUGIN ${ARG_PLUGINS})
                    target_link_libraries(${TEST_NAME}
                        ${ARG_PLUGIN}Plugin
                    )
                endforeach()

                # Qt modules

                foreach(ARG_QT_MODULE ${ARG_QT_MODULES} Test)
                    target_link_libraries(${TEST_NAME}
                        Qt5::${ARG_QT_MODULE}
                    )
                endforeach()

                # External binaries

                if(NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL "")
                    foreach(ARG_EXTERNAL_BINARY ${ARG_EXTERNAL_BINARIES})
                        set(FULL_EXTERNAL_BINARY "${ARG_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}")

                        if(WIN32)
                            string(REGEX REPLACE "${CMAKE_SHARED_LIBRARY_SUFFIX}$" "${CMAKE_IMPORT_LIBRARY_SUFFIX}"
                                   IMPORT_EXTERNAL_BINARY "${FULL_EXTERNAL_BINARY}")

                            target_link_libraries(${TEST_NAME}
                                ${IMPORT_EXTERNAL_BINARY}
                            )
                        elseif(APPLE)
                            target_link_libraries(${TEST_NAME}
                                ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${ARG_EXTERNAL_BINARY}
                            )
                        else()
                            target_link_libraries(${TEST_NAME}
                                ${FULL_EXTERNAL_BINARY}
                            )
                        endif()
                    endforeach()
                endif()

                # System binaries

                foreach(ARG_SYSTEM_BINARY ${ARG_SYSTEM_BINARIES})
                    target_link_libraries(${TEST_NAME}
                        ${ARG_SYSTEM_BINARY}
                    )
                endforeach()

                # Add some dependencies, if any

                if(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
                    add_dependencies(${TEST_NAME} ${ARG_DEPENDS_ON})
                endif()

                foreach(ARG_PLUGIN ${ARG_PLUGINS})
                    add_dependencies(${TEST_NAME} ${ARG_PLUGIN}Plugin)
                endforeach()

                # Copy the test to our tests directory
                # Note: DEST_TESTS_DIR is defined in our main CMake file...

                set(TEST_FILENAME ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})

                if(WIN32)
                    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${TEST_FILENAME}
                                                                        ${PROJECT_BUILD_DIR}/${TEST_FILENAME})
                endif()

                add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${TEST_FILENAME}
                                                                    ${DEST_TESTS_DIR}/${TEST_FILENAME})

                # Clean up our plugin's tests, if we are on macOS

                if(APPLE)
                    macos_clean_up_file_with_qt_dependencies(${TEST_NAME} ${DEST_TESTS_DIR} ${TEST_FILENAME})
                endif()
            else()
                message(AUTHOR_WARNING "The '${ARG_TEST}' test for the '${PLUGIN_NAME}' plugin does not exist...")
            endif()
        endforeach()
    endif()
endmacro()

#===============================================================================

macro(copy_file_to_build_dir PROJECT_TARGET ORIG_DIRNAME DEST_DIRNAME FILENAME)
    # Copy the file (renaming it, if needed) to the destination folder
    # Note: DIRECT is used to copy a file that doesn't first need to be built.
    #       This means that we can then use execute_process() rather than
    #       add_custom_command(), and thus reduce the length of the
    #       PROJECT_TARGET command, something that can be useful on Windows
    #       since the command might otherwise end up being too long for Windows
    #       to handle...

    if("${ARGN}" STREQUAL "")
        set(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIRNAME}/${FILENAME})
    else()
        # An argument was passed so use it to rename the file, which is to be
        # copied

        set(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIRNAME}/${ARGN})
    endif()

    if("${PROJECT_TARGET}" STREQUAL "DIRECT")
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                         ${FULL_DEST_FILENAME})
    else()
        add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${FULL_DEST_FILENAME}
                           BYPRODUCTS ${FULL_DEST_FILENAME})
    endif()
endmacro()

#===============================================================================

macro(windows_deploy_qt_library LIBRARY_NAME)
    # Copy the Qt library to both the build and build/bin folders, so we can
    # test things both from within Qt Creator and without first having to deploy
    # OpenCOR

    if(   "${LIBRARY_NAME}" STREQUAL "Qt5WebKit"
       OR "${LIBRARY_NAME}" STREQUAL "Qt5WebKitWidgets"
       OR "${LIBRARY_NAME}" STREQUAL "icudt${ICU_VERSION}"
       OR "${LIBRARY_NAME}" STREQUAL "icuin${ICU_VERSION}"
       OR "${LIBRARY_NAME}" STREQUAL "icuuc${ICU_VERSION}")
        set(REAL_QT_BINARY_DIR ${QTWEBKIT_BINARIES_DIR})
    else()
        set(REAL_QT_BINARY_DIR ${QT_BINARY_DIR})
    endif()

    set(LIBRARY_RELEASE_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    if("${LIBRARY_NAME}" STREQUAL "icudt${ICU_VERSION}")
        set(LIBRARY_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}icudtd${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    elseif("${LIBRARY_NAME}" STREQUAL "icuin${ICU_VERSION}")
        set(LIBRARY_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}icuind${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    elseif("${LIBRARY_NAME}" STREQUAL "icuuc${ICU_VERSION}")
        set(LIBRARY_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}icuucd${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    else()
        set(LIBRARY_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()

    if(NOT EXISTS ${REAL_QT_BINARY_DIR}/${LIBRARY_DEBUG_FILENAME})
        # No debug version of the Qt library exists, so use its release version
        # instead

        set(LIBRARY_DEBUG_FILENAME ${LIBRARY_RELEASE_FILENAME})
    endif()

    if(RELEASE_MODE)
        set(LIBRARY_FILENAME ${LIBRARY_RELEASE_FILENAME})
    else()
        set(LIBRARY_FILENAME ${LIBRARY_DEBUG_FILENAME})
    endif()

    copy_file_to_build_dir(DIRECT ${REAL_QT_BINARY_DIR} . ${LIBRARY_FILENAME})
    copy_file_to_build_dir(DIRECT ${REAL_QT_BINARY_DIR} bin ${LIBRARY_FILENAME})

    # Deploy the Qt library

    install(FILES ${REAL_QT_BINARY_DIR}/${LIBRARY_FILENAME}
            DESTINATION bin)
endmacro()

#===============================================================================

macro(windows_deploy_qt_plugin PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        set(PLUGIN_ORIG_DIRNAME ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        set(PLUGIN_DEST_DIRNAME plugins/${PLUGIN_CATEGORY})
        set(PLUGIN_RELEASE_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(PLUGIN_DEBUG_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX})

        if(RELEASE_MODE)
            set(PLUGIN_FILENAME ${PLUGIN_RELEASE_FILENAME})
        else()
            set(PLUGIN_FILENAME ${PLUGIN_DEBUG_FILENAME})
        endif()

        copy_file_to_build_dir(DIRECT ${PLUGIN_ORIG_DIRNAME} ${PLUGIN_DEST_DIRNAME} ${PLUGIN_FILENAME})

        # Deploy the Qt plugin

        install(FILES ${PLUGIN_ORIG_DIRNAME}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIRNAME})
    endforeach()
endmacro()

#===============================================================================

macro(runpath2rpath PROJECT_TARGET FILENAME)
    # Convert the RUNPATH value, if any, of the given ELF file to an RPATH value

    if("${PROJECT_TARGET}" STREQUAL "DIRECT")
        execute_process(COMMAND ${RUNPATH2RPATH} ${FILENAME}
                        RESULT_VARIABLE RESULT)

        if(NOT RESULT EQUAL 0)
            message(FATAL_ERROR "The RUNPATH value of '${FILENAME}' could not be converted to an RPATH value...")
        endif()
    else()
        add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND ${RUNPATH2RPATH} ${FILENAME})
    endif()
endmacro()

#===============================================================================

macro(linux_deploy_qt_library PROJECT_TARGET DIRNAME FILENAME)
    # Copy the Qt library to the build/lib folder, so we can test things without
    # first having to deploy OpenCOR
    # Note: this is particularly useful when the Linux machine has different
    #       versions of Qt...

    copy_file_to_build_dir(${PROJECT_TARGET} ${DIRNAME} lib ${FILENAME})

    # Make sure that the RUNPATH value is converted to an RPATH value

    runpath2rpath(${PROJECT_TARGET} ${PROJECT_BUILD_DIR}/lib/${FILENAME})

    # Strip the Qt library of all its local symbols

    if(RELEASE_MODE)
        if("${PROJECT_TARGET}" STREQUAL "DIRECT")
            execute_process(COMMAND strip -x ${PROJECT_BUILD_DIR}/lib/${FILENAME})
        else()
            add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND strip -x ${PROJECT_BUILD_DIR}/lib/${FILENAME})
        endif()
    endif()

    # Deploy the Qt library

    install(FILES ${PROJECT_BUILD_DIR}/lib/${FILENAME}
            DESTINATION lib)
endmacro()

#===============================================================================

macro(linux_deploy_qt_plugin PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        set(PLUGIN_ORIG_DIRNAME ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        set(PLUGIN_DEST_DIRNAME plugins/${PLUGIN_CATEGORY})
        set(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        copy_file_to_build_dir(DIRECT ${PLUGIN_ORIG_DIRNAME} ${PLUGIN_DEST_DIRNAME} ${PLUGIN_FILENAME})

        # Make sure that the RUNPATH value is converted to an RPATH value

        runpath2rpath(DIRECT ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME})

        # Strip the Qt plugin of all its local symbols

        if(RELEASE_MODE)
            execute_process(COMMAND strip -x ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME})
        endif()

        # Deploy the Qt plugin

        install(FILES ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIRNAME}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIRNAME})
    endforeach()
endmacro()

#===============================================================================

macro(linux_deploy_system_library FILENAME NEW_FILENAME)
    # Copy the system library to the build/lib folder, so we can test things
    # without first having to deploy OpenCOR

    get_filename_component(REAL_FULL_FILENAME ${FILENAME} REALPATH)
    get_filename_component(REAL_DIRNAME ${REAL_FULL_FILENAME} DIRECTORY)
    get_filename_component(REAL_FILENAME ${REAL_FULL_FILENAME} NAME)

    copy_file_to_build_dir(DIRECT ${REAL_DIRNAME} lib ${REAL_FILENAME} ${NEW_FILENAME})

    # Deploy the system library

    install(FILES ${REAL_FULL_FILENAME}
            DESTINATION lib
            RENAME ${NEW_FILENAME})
endmacro()

#===============================================================================

macro(macos_clean_up_file PROJECT_TARGET DIRNAME FILENAME)
    # Strip the file of all its local symbols

    set(FULL_FILENAME ${DIRNAME}/${FILENAME})

    if(RELEASE_MODE)
        if("${PROJECT_TARGET}" STREQUAL "DIRECT")
            execute_process(COMMAND strip -x ${FULL_FILENAME})
        else()
            add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND strip -x ${FULL_FILENAME})
        endif()
    endif()

    # Clean up the file's id

    if("${PROJECT_TARGET}" STREQUAL "DIRECT")
        execute_process(COMMAND install_name_tool -id @rpath/${FILENAME} ${FULL_FILENAME})
    else()
        add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND install_name_tool -id @rpath/${FILENAME} ${FULL_FILENAME})
    endif()
endmacro()

#===============================================================================

macro(macos_clean_up_file_with_qt_dependencies PROJECT_TARGET DIRNAME FILENAME)
    # Clean up the file

    macos_clean_up_file(${PROJECT_TARGET} ${DIRNAME} ${FILENAME})

    # Make sure that the file refers to our embedded copy of the Qt libraries

    foreach(MACOS_QT_LIBRARY ${MACOS_QT_LIBRARIES})
        set(MACOS_QT_LIBRARY_FILENAME ${MACOS_QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${MACOS_QT_LIBRARY})

        set(OLD_REFERENCE ${QT_LIBRARY_DIR}/${MACOS_QT_LIBRARY_FILENAME})
        set(NEW_REFERENCE @rpath/${MACOS_QT_LIBRARY_FILENAME})

        if("${PROJECT_TARGET}" STREQUAL "DIRECT")
            execute_process(COMMAND install_name_tool -change ${OLD_REFERENCE} ${NEW_REFERENCE} ${FULL_FILENAME})
        else()
            add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND install_name_tool -change ${OLD_REFERENCE} ${NEW_REFERENCE} ${FULL_FILENAME})
        endif()
    endforeach()
endmacro()

#===============================================================================

macro(macos_deploy_qt_file ORIG_DIRNAME DEST_DIRNAME FILENAME)
    # Copy the Qt file

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                     ${DEST_DIRNAME}/${FILENAME})

    # Clean up the Qt file

    macos_clean_up_file_with_qt_dependencies(DIRECT ${DEST_DIRNAME} ${FILENAME})
endmacro()

#===============================================================================

macro(macos_deploy_qt_library LIBRARY_NAME)
    # Deploy the Qt library

    set(QT_FRAMEWORK_DIR ${LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR})

    if(   "${LIBRARY_NAME}" STREQUAL "${QTWEBKIT}"
       OR "${LIBRARY_NAME}" STREQUAL "${QTWEBKITWIDGETS}")
        set(REAL_QT_LIBRARY_DIR ${QTWEBKIT_LIBRARIES_DIR})
    else()
        set(REAL_QT_LIBRARY_DIR ${QT_LIBRARY_DIR})
    endif()

    macos_deploy_qt_file(${REAL_QT_LIBRARY_DIR}/${QT_FRAMEWORK_DIR}
                         ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks/${QT_FRAMEWORK_DIR}
                         ${LIBRARY_NAME})
endmacro()

#===============================================================================

macro(macos_deploy_qt_plugin PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin

        macos_deploy_qt_file(${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}
                             ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${PLUGIN_CATEGORY}
                             ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    endforeach()
endmacro()

#===============================================================================

macro(create_package_file PACKAGE_NAME PACKAGE_VERSION DIRNAME)
    # Various initialisations

    set(OPTIONS)
    set(ONE_VALUE_KEYWORDS
        PACKAGE_REPOSITORY
        RELEASE_TAG
        TARGET_PLATFORM
        TARGET
    )
    set(MULTI_VALUE_KEYWORDS
        PACKAGED_FILES
        SHA1_FILES
    )

    cmake_parse_arguments(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Make sure that we have at least one file for which we want to check the
    # SHA-1 value

    list(LENGTH ARG_SHA1_FILES ARG_SHA1_FILES_COUNT)

    if(ARG_SHA1_FILES_COUNT EQUAL 0)
        message(FATAL_ERROR "At least one file must have its SHA-1 value calculated in order to create the '${PACKAGE_NAME}' package...")
    endif()

    # The full path to the package's files

    set(REAL_DIRNAME "${PROJECT_SOURCE_DIR}/${DIRNAME}")

    # Remove any historical package archive

    if(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
        set(REAL_TARGET_PLATFORM ${ARG_TARGET_PLATFORM})
    else()
        set(REAL_TARGET_PLATFORM ${TARGET_PLATFORM})
    endif()

    set(COMPRESSED_FILENAME ${PROJECT_BUILD_DIR}/${PACKAGE_NAME}.${PACKAGE_VERSION}.${REAL_TARGET_PLATFORM}.tar.gz)

    file(REMOVE ${COMPRESSED_FILENAME})

    # The actual packaging code goes into a separate CMake script file that is
    # run as a POST_BUILD step

    set(CMAKE_CODE "cmake_minimum_required(VERSION 3.3)

# Files and directories to package

set(PACKAGED_FILES")

    foreach(FILENAME IN LISTS ARG_PACKAGED_FILES)
        set(CMAKE_CODE "${CMAKE_CODE}\n    ${FILENAME}")
    endforeach()

    set(CMAKE_CODE "${CMAKE_CODE}\n)

# Files to have their SHA-1 value checked

set(SHA1_FILES")

    foreach(FILENAME IN LISTS ARG_SHA1_FILES)
        set(CMAKE_CODE "${CMAKE_CODE}\n    ${FILENAME}")
    endforeach()

    set(CMAKE_CODE "${CMAKE_CODE}\n)

# Calculate the SHA-1 value of our different files, after having stripped them,
# if needed

set(SHA1_VALUES)

foreach(SHA1_FILE IN LISTS SHA1_FILES)
    set(REAL_SHA1_FILENAME \"${REAL_DIRNAME}/\$\{SHA1_FILE\}\")

    if(NOT EXISTS \$\{REAL_SHA1_FILENAME\})
        message(FATAL_ERROR \"'\$\{REAL_SHA1_FILENAME\}' is missing from the '${PACKAGE_NAME}' package...\")
    endif()

    if(NOT WIN32 AND RELEASE_MODE)
        execute_process(COMMAND strip -x \$\{REAL_SHA1_FILENAME\})
    endif()

    file(SHA1 \$\{REAL_SHA1_FILENAME\} SHA1_VALUE)

    list(APPEND SHA1_VALUES \$\{SHA1_VALUE\})
endforeach()

# Compress our package

execute_process(COMMAND ${CMAKE_COMMAND} -E tar -czf ${COMPRESSED_FILENAME} \$\{PACKAGED_FILES\}
                WORKING_DIRECTORY ${REAL_DIRNAME}
                OUTPUT_QUIET)

# Make sure that the compressed version of our package exists

if(EXISTS ${COMPRESSED_FILENAME})
    # The compressed version of our package exists, so calculate its SHA-1 value
    # and let people know how we should call the retrieve_package_file() macro

    file(SHA1 ${COMPRESSED_FILENAME} SHA1_VALUE)

    string(REPLACE \"\;\" \"\\n                                  \"
           SHA1_VALUES \"\$\{SHA1_VALUES\}\")

    message(\"To retrieve the '${PACKAGE_NAME}' package, use:
retrieve_package_file(\\$\\{PACKAGE_NAME\\} \\$\\{PACKAGE_VERSION\\}
                      \\$\\{RELATIVE_PROJECT_SOURCE_DIR\\} \$\{SHA1_VALUE\}")

    if(NOT "${ARG_PACKAGE_REPOSITORY}" STREQUAL "")
        set(CMAKE_CODE "${CMAKE_CODE}\n                      PACKAGE_REPOSITORY \\$\\{PACKAGE_REPOSITORY\\}")
    endif()

    if(NOT "${ARG_RELEASE_TAG}" STREQUAL "")
        set(CMAKE_CODE "${CMAKE_CODE}\n                      RELEASE_TAG \\$\\{RELEASE_TAG\\}")
    endif()

    if(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
        set(CMAKE_CODE "${CMAKE_CODE}\n                      TARGET_PLATFORM \\$\\{TARGET_PLATFORM\\}")
    endif()

    set(CMAKE_CODE "${CMAKE_CODE}\n                      SHA1_FILES \\$\\{SHA1_FILES\\}
                      SHA1_VALUES \$\{SHA1_VALUES\})\")
else()
    message(FATAL_ERROR \"The compressed version of the '${PACKAGE_NAME}' package could not be generated...\")
endif()
")

    set(PACKAGING_SCRIPT ${PROJECT_BINARY_DIR}/package${PACKAGE_NAME}.cmake)

    file(WRITE ${PACKAGING_SCRIPT} ${CMAKE_CODE})

    # Run the packaging script once the dependency target has been satisfied

    add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -D RELEASE_MODE=${RELEASE_MODE} -P ${PACKAGING_SCRIPT}
                       VERBATIM)
endmacro()

#===============================================================================

macro(check_files DIRNAME FILENAMES SHA1_VALUES)
    # By default, everything is OK

    set(CHECK_FILES_OK TRUE)

    # See our parameters as lists

    set(FILENAMES_LIST ${FILENAMES})
    set(SHA1_VALUES_LIST ${SHA1_VALUES})

    # Retrieve the number of SHA-1 files and values we have

    list(LENGTH FILENAMES_LIST FILENAMES_COUNT)

    # Make sure that we have some files

    if(FILENAMES_COUNT)
        # Determine our range

        math(EXPR RANGE "${FILENAMES_COUNT}-1")

        # Make sure that our files, if they exist, have the expected SHA-1 value

        foreach(I RANGE ${RANGE})
            list(GET FILENAMES_LIST ${I} FILENAME)
            list(GET SHA1_VALUES_LIST ${I} SHA1_VALUE)

            set(REAL_FILENAME ${DIRNAME}/${FILENAME})

            if(EXISTS ${REAL_FILENAME})
                file(SHA1 ${REAL_FILENAME} REAL_SHA1_VALUE)

                if(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
                    # The file doesn't have the expected SHA-1 value, so remove
                    # it and fail the checks

                    file(REMOVE ${REAL_FILENAME})

                    set(CHECK_FILES_OK FALSE)
                endif()
            elseif(CHECK_FILES_OK)
                # The file is missing, so fail the checks

                set(CHECK_FILES_OK FALSE)
            endif()
        endforeach()
    endif()
endmacro()

#===============================================================================

macro(check_file DIRNAME FILENAME SHA1_VALUE)
    # Convenience macro

    check_files(${DIRNAME} ${FILENAME} ${SHA1_VALUE})

    set(CHECK_FILE_OK ${CHECK_FILES_OK})
endmacro()

#===============================================================================

macro(retrieve_package_file PACKAGE_NAME PACKAGE_VERSION DIRNAME SHA1_VALUE)
    # Various initialisations

    set(OPTIONS)
    set(ONE_VALUE_KEYWORDS
        PACKAGE_REPOSITORY
        RELEASE_TAG
        TARGET_PLATFORM
    )
    set(MULTI_VALUE_KEYWORDS
        SHA1_FILES
        SHA1_VALUES
    )

    cmake_parse_arguments(ARG "${OPTIONS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

    # Make sure that we have at least one file for which we need to check the
    # SHA-1 value

    list(LENGTH ARG_SHA1_FILES ARG_SHA1_FILES_COUNT)
    list(LENGTH ARG_SHA1_VALUES ARG_SHA1_VALUES_COUNT)

    if(       ARG_SHA1_FILES_COUNT EQUAL 0
       OR NOT ARG_SHA1_FILES_COUNT EQUAL ARG_SHA1_VALUES_COUNT)
        message(FATAL_ERROR "At least one file must have its SHA-1 value checked in order to retrieve the '${PACKAGE_NAME}' package...")
    endif()

    # Create our destination folder, if needed

    set(REAL_DIRNAME ${CMAKE_SOURCE_DIR}/${DIRNAME}/ext)

    string(REGEX REPLACE "${REMOTE_EXTERNAL_PACKAGE_DIR}/ext$" "${LOCAL_EXTERNAL_PACKAGE_DIR}"
           REAL_DIRNAME "${REAL_DIRNAME}")
    string(REGEX REPLACE "${PLATFORM_DIR}/ext$" "ext"
           REAL_DIRNAME "${REAL_DIRNAME}")

    if(NOT EXISTS ${REAL_DIRNAME})
        file(MAKE_DIRECTORY ${REAL_DIRNAME})
    endif()

    # Make sure that we have the expected package's files

    check_files(${REAL_DIRNAME} "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

    if(NOT CHECK_FILES_OK)
        # Something went wrong with the package's files, so retrieve the package

        message("Retrieving the '${PACKAGE_NAME}' package...")

        if(NOT "${ARG_TARGET_PLATFORM}" STREQUAL "")
            set(REAL_TARGET_PLATFORM ${ARG_TARGET_PLATFORM})
        else()
            set(REAL_TARGET_PLATFORM ${TARGET_PLATFORM})
        endif()

        set(COMPRESSED_FILENAME ${PACKAGE_NAME}.${PACKAGE_VERSION}.${REAL_TARGET_PLATFORM}.tar.gz)
        set(FULL_COMPRESSED_FILENAME ${REAL_DIRNAME}/${COMPRESSED_FILENAME})

        if("${ARG_PACKAGE_REPOSITORY}" STREQUAL "")
            string(TOLOWER ${PACKAGE_NAME} PACKAGE_REPOSITORY)
        else()
            set(PACKAGE_REPOSITORY ${ARG_PACKAGE_REPOSITORY})
        endif()

        if("${ARG_RELEASE_TAG}" STREQUAL "")
            set(RELEASE_TAG v${PACKAGE_VERSION})
        else()
            set(RELEASE_TAG ${ARG_RELEASE_TAG})
        endif()

        file(DOWNLOAD "https://github.com/opencor/${PACKAGE_REPOSITORY}/releases/download/${RELEASE_TAG}/${COMPRESSED_FILENAME}" ${FULL_COMPRESSED_FILENAME}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the compressed version of the package, should we have
        # managed to retrieve it

        list(GET STATUS 0 STATUS_CODE)

        if(${STATUS_CODE} EQUAL 0)
            check_file(${REAL_DIRNAME} ${COMPRESSED_FILENAME} ${SHA1_VALUE})

            if(NOT CHECK_FILE_OK)
                message(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package does not have the expected SHA-1 value...")
            endif()

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${FULL_COMPRESSED_FILENAME}
                            WORKING_DIRECTORY ${REAL_DIRNAME}
                            OUTPUT_QUIET)

            file(REMOVE ${FULL_COMPRESSED_FILENAME})
        else()
            file(REMOVE ${FULL_COMPRESSED_FILENAME})
            # Note: this is in case we had an HTTP error of sorts, in which case
            #       we would end up with an empty file...

            message(FATAL_ERROR "The compressed version of the '${PACKAGE_NAME}' package could not be retrieved...")
        endif()

        # Check that the package's files, if we managed to uncompress the
        # package, have the expected SHA-1 values

        list(GET STATUS 0 STATUS_CODE)

        if(${STATUS_CODE} EQUAL 0)
            check_files(${REAL_DIRNAME} "${ARG_SHA1_FILES}" "${ARG_SHA1_VALUES}")

            if(NOT CHECK_FILES_OK)
                message(FATAL_ERROR "The files in the '${PACKAGE_NAME}' package do not have the expected SHA-1 values...")
            endif()
        else()
            message(FATAL_ERROR "The files in the '${PACKAGE_NAME}' package could not be uncompressed...")
        endif()
    endif()
endmacro()
