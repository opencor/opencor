set(LANGUAGES fr)

#===============================================================================

macro(configure_clang_and_clang_tidy TARGET_NAME)
    # Configure Clang and Clang-Tidy for the given target

    if(APPLE)
        # Note #1: the full list of diagnostic flags for Clang can be found at
        #          https://clang.llvm.org/docs/DiagnosticsReference.html...
        # Note #2: besides C++98 compatibility, which we are not after, some
        #          warnings are disabled so that Qt-related files can be
        #          compiled without any problems...
        # Note #3: no-anon-enum-enum-conversion is disabled so that we can
        #          safely use Type.h from Clang...

        set(COMPILE_OPTIONS
            -Weverything
            -Wno-anon-enum-enum-conversion
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-documentation
            -Wno-exit-time-destructors
            -Wno-extra-semi-stmt
            -Wno-global-constructors
            -Wno-header-hygiene
            -Wno-missing-prototypes
            -Wno-non-c-typedef-for-linkage
            -Wno-padded
            -Wno-redundant-parens
            -Wno-sign-conversion
            -Wno-suggest-destructor-override
            -Wno-suggest-override
            -Wno-unknown-warning-option
            -Wno-used-but-marked-unused
            -Wno-zero-as-null-pointer-constant
        )

        set_target_properties(${TARGET_NAME} PROPERTIES
            COMPILE_OPTIONS "${COMPILE_OPTIONS}"
        )
    endif()

    if(ENABLE_CLANG_TIDY)
        # Note: Clang-Tidy does, by default, only report issues with C++ files.
        #       When it comes to header files, we must specify the ones for
        #       which we want issues to be reported, this using `-header-filter`
        #       and/or `-line-filter`. The problem is that the format of the
        #       path reported by Clang-Tidy depends on the CMake generator that
        #       is used. For example, with Ninja, we have a path that looks like
        #       /a/b/build/../src while with Make it looks like /a/b/src. Also,
        #       the regular expression needed by `-header-filter` follows the
        #       POSIX ERE standard rather than the PCRE one, which is a big
        #       limitation. Indeed, we are not interested in some header files
        #       that are located in specific folders. Using a PCRE regular
        #       expression, we could easily filter them out with something like
        #       ^(?:(?!xxx|yyy|zzz).)*$, i.e. a regular expression with a
        #       non-capturing group and a negative lookahead, but we can't since
        #       we need a POSIX ERE regulard expression. So, in the end, we ask
        #       for issues related to header files located under ../src, which
        #       means that it only works with Ninja and from [OpenCOR]/build.
        #       Then, there are some header files that are under [OpenCOR]/src,
        #       but for which we don't want to see issues. So, we filter them
        #       out using `-line-filter`, which is normally used to show issues
        #       present between a given set of lines (!?). So, here, we ask for
        #       issues between lines 9999999 and 9999999 (!!). We finish our
        #       list with `.h`, so that issues with other header files can get
        #       reported...

        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY};-header-filter=\\\.\\\.\\\/src\\\/.*;-line-filter=[{'name':'diff_match_patch.h','lines':[[9999999,9999999]]},{'name':'qzipreader_p.h','lines':[[9999999,9999999]]},{'name':'.h'}]"
        )
    endif()
endmacro()

#===============================================================================

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
    # Note: we use ERROR_QUIET with lupdate because it does generate some
    #       "errors" for our .cpp.inl files even though everything is fine with
    #       them...

    set(INPUT_FILES)

    foreach(INPUT_FILE ${ARGN})
        list(APPEND INPUT_FILES ${INPUT_FILE})
    endforeach()

    foreach(LANGUAGE ${LANGUAGES})
        set(LANGUAGE_FILE ${TARGET_NAME}_${LANGUAGE})
        set(TS_FILE i18n/${LANGUAGE_FILE}.ts)
        set(QM_FILE ${PROJECT_BUILD_DIR}/${LANGUAGE_FILE}.qm)

        if(EXISTS ${PROJECT_SOURCE_DIR}/${TS_FILE})
            execute_process(COMMAND ${QT_BINARIES_DIR}/lupdate -no-obsolete ${INPUT_FILES}
                                                               -ts ${TS_FILE}
                                                               -I ${CMAKE_SOURCE_DIR}/src/misc
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                            OUTPUT_QUIET
                            ERROR_QUIET)
            execute_process(COMMAND ${QT_BINARIES_DIR}/lrelease ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                                -qm ${QM_FILE}
                            OUTPUT_QUIET)

            track_files(${QM_FILE})
        endif()
    endforeach()
endmacro()

#===============================================================================

macro(check_python_package PACKAGE AVAILABLE)
    set(PACKAGE_VARIABLE "HAS_PYTHON_${PACKAGE}")

    string(TOUPPER "${PACKAGE_VARIABLE}" PACKAGE_VARIABLE)
    string(REPLACE "-" "_" PACKAGE_VARIABLE "${PACKAGE_VARIABLE}")

    message(STATUS "Performing Test ${PACKAGE_VARIABLE}")

    if(Python_EXECUTABLE)
        execute_process(COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cmake/check_python_package.py ${PACKAGE}
                        RESULT_VARIABLE RESULT
                        OUTPUT_QUIET ERROR_QUIET)
    endif()

    if(Python_EXECUTABLE AND RESULT EQUAL 0)
        set(${AVAILABLE} TRUE)

        message(STATUS "Performing Test ${PACKAGE_VARIABLE} - Success")
    else()
        message(STATUS "Performing Test ${PACKAGE_VARIABLE} - Failed")
    endif()
endmacro()

#===============================================================================

macro(generate_documentation BUILD_OPENCOR)
    # General documentation

    ExternalProject_Add(GeneralDocumentationBuild
        GIT_REPOSITORY
            https://github.com/opencor/general-documentation
        GIT_SHALLOW
        CMAKE_ARGS
            -DMODE=${CMAKE_PROJECT_NAME}
            -DENABLE_DOWNLOADS=${BUILD_OPENCOR}
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/GeneralDocumentationBuild-prefix/src/GeneralDocumentationBuild-build/html
                                               ${CMAKE_BINARY_DIR}/doc/html
    )

    if(${BUILD_OPENCOR})
        configure_file(${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp.in
                       ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp)

        file(COPY ${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhp DESTINATION ${PROJECT_BUILD_DIR}/doc)
    endif()

    # User and developer documentations

    build_documentation(user ${BUILD_OPENCOR})
    build_documentation(developer ${BUILD_OPENCOR})
endmacro()

#===============================================================================

macro(build_documentation DOCUMENTATION_NAME BUILD_OPENCOR)
    # Build the given documentation as an external project and have it copied to
    # our final documentation directory

    set(DOCUMENTATION_BUILD ${DOCUMENTATION_NAME}DocumentationBuild)

    if(${BUILD_OPENCOR})
        set(DOCUMENTATION_CMAKE_ARGS
            -DPYTHON_EXECUTABLE=${Python_EXECUTABLE}
            -DSPHINX_EXECUTABLE=${SPHINX_EXECUTABLE}
        )
    endif()

    ExternalProject_Add(${DOCUMENTATION_BUILD}
        GIT_REPOSITORY
            https://github.com/opencor/opencor-${DOCUMENTATION_NAME}-documentation
        GIT_SHALLOW
        CMAKE_ARGS
            ${DOCUMENTATION_CMAKE_ARGS}
        LIST_SEPARATOR
            |
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${DOCUMENTATION_BUILD}-prefix/src/${DOCUMENTATION_BUILD}-build/html
                                               ${CMAKE_BINARY_DIR}/doc/html/${DOCUMENTATION_NAME}
    )

    # Make our documentation build target depend on our local target, if we are
    # building OpencOR

    if(${BUILD_OPENCOR})
        add_dependencies(${DOCUMENTATION_BUILD_TARGET} ${DOCUMENTATION_BUILD})
    endif()
endmacro()

#===============================================================================

macro(strip_file PROJECT_TARGET FILENAME)
    # Strip the given file of all its local symbols, if we are in release mode
    # Note: to strip QScintilla and Qwt when building them on Linux results in
    #       an error due to patchelf having been used on them. So, we use a
    #       wrapper that ignores errors and returns 0, so that our build doesn't
    #       break...

    if(RELEASE_MODE)
        if("${PROJECT_TARGET}" STREQUAL "DIRECT")
            execute_process(COMMAND strip -x ${FILENAME})
        else()
            add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                               COMMAND strip -x ${FILENAME})
        endif()
    endif()
endmacro()

#===============================================================================

macro(add_plugin PLUGIN_NAME)
    # Various initialisations

    set(PLUGIN_NAME ${PLUGIN_NAME})

    set(OPTIONS
        NO_STRIP
    )
    set(ONE_VALUE_KEYWORDS
        EXTERNAL_BINARIES_DIR
        EXTERNAL_SOURCE_DIR
        EXTERNAL_DESTINATION_DIR
    )
    set(MULTI_VALUE_KEYWORDS
        SOURCES
        UIS
        PLUGINS
        QT_MODULES
        EXTERNAL_BINARIES
        SYSTEM_BINARIES
        DEPENDS_ON
        BYPRODUCTS
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

        configure_file(${I18N_QRC_IN_FILENAME} ${I18N_QRC_FILENAME})

        list(APPEND RESOURCES ${I18N_QRC_FILENAME})

        # Update the translation (.ts) files and generate the language (.qm)
        # files, which will later on be embedded in the plugin

        update_language_files(${PLUGIN_NAME} ${ARG_SOURCES} ${ARG_UIS})
    endif()

    if(EXISTS ${UI_QRC_FILENAME})
        list(APPEND RESOURCES ${UI_QRC_FILENAME})
    endif()

    # Definition to make sure that the plugin can be used by other plugins

    add_compile_definitions(${PLUGIN_NAME}_PLUGIN)

    # On Linux, set the RPATH value to use by the plugin

    if(NOT WIN32 AND NOT APPLE)
        set(PLUGIN_LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/../../lib'")

        string(REPLACE "${LINK_RPATH_FLAG}" "${PLUGIN_LINK_RPATH_FLAG}"
               LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
    endif()

    # Generate and add the different files needed by the plugin

    add_library(${PROJECT_NAME} SHARED
        ${ARG_SOURCES}
        ${RESOURCES}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    configure_clang_and_clang_tidy(${PROJECT_NAME})

    # OpenCOR plugins

    foreach(ARG_PLUGIN ${ARG_PLUGINS})
        target_link_libraries(${PROJECT_NAME}
            ${ARG_PLUGIN}Plugin
        )
    endforeach()

    # Qt modules

    foreach(ARG_QT_MODULE ${ARG_QT_MODULES})
        if(NOT APPLE)
            find_package(Qt5${ARG_QT_MODULE} REQUIRED)
        endif()

        target_link_libraries(${PROJECT_NAME}
            Qt5::${ARG_QT_MODULE}
        )
    endforeach()

    # Create a custom target for copying external binaries
    # Note #1: this is to prevent Ninja from getting confused with circular
    #          references...
    # Note #2: we use this target in the PythonQt build script when setting the
    #          PYTHON_DEPENDENCIES list...

    if(   NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL ""
       OR (    NOT "${ARG_EXTERNAL_SOURCE_DIR}" STREQUAL ""
           AND NOT "${ARG_EXTERNAL_DESTINATION_DIR}" STREQUAL ""))
        set(COPY_EXTERNAL_BINARIES_TARGET "COPY_${PROJECT_NAME}_EXTERNAL_BINARIES")

        add_custom_target(${COPY_EXTERNAL_BINARIES_TARGET})
        add_dependencies(${PROJECT_NAME} ${COPY_EXTERNAL_BINARIES_TARGET})

        if(NOT "${ARG_DEPENDS_ON}" STREQUAL "")
            add_dependencies(${COPY_EXTERNAL_BINARIES_TARGET} ${ARG_DEPENDS_ON})
        endif()
    endif()

    # External binaries

    if(NOT "${ARG_EXTERNAL_BINARIES_DIR}" STREQUAL "")
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

            copy_file_to_build_dir(${COPY_TARGET} ${ARG_EXTERNAL_BINARIES_DIR} ${DEST_EXTERNAL_LIBRARIES_DIR} ${ARG_EXTERNAL_BINARY})

            # Strip the external library of all its local symbols, if possible

            if(NOT WIN32 AND NOT ARG_NO_STRIP)
                strip_file(${COPY_TARGET} ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY})
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
                    ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY}
                )
            else()
                target_link_libraries(${PROJECT_NAME}
                    ${FULL_EXTERNAL_BINARY}
                )
            endif()

            # On macOS, ensure that @rpath is set in the external library's id
            # and that it references the correct Qt libraries, if any at all

            if(APPLE)
                if("${COPY_TARGET}" STREQUAL "DIRECT")
                    execute_process(COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY})
                else()
                    add_custom_command(TARGET ${COPY_TARGET} POST_BUILD
                                       COMMAND install_name_tool -id @rpath/${ARG_EXTERNAL_BINARY} ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY})
                endif()

                strip_file(${COPY_TARGET} ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY})
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

    if(    NOT "${ARG_EXTERNAL_SOURCE_DIR}" STREQUAL ""
       AND NOT "${ARG_EXTERNAL_DESTINATION_DIR}" STREQUAL "")
        # Copy the entire source directory to the destination

        add_custom_command(TARGET ${COPY_EXTERNAL_BINARIES_TARGET}
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_EXTERNAL_SOURCE_DIR}
                                                                      ${ARG_EXTERNAL_DESTINATION_DIR}
                           BYPRODUCTS ${ARG_BYPRODUCTS})
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
        strip_file(${PROJECT_NAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})
    elseif(NOT WIN32)
        runpath2rpath(${PROJECT_NAME} ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME})
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
            set(TEST_HEADER tests/${ARG_TEST}.h)

            if(    EXISTS ${PROJECT_SOURCE_DIR}/${TEST_SOURCE}
               AND EXISTS ${PROJECT_SOURCE_DIR}/${TEST_HEADER})
                # The test exists, so build it, but first set the RPATH and
                # RPATH link values to use by the test, if on Linux

                if(NOT WIN32 AND NOT APPLE)
                    string(REPLACE "${PLUGIN_LINK_RPATH_FLAG}" "-Wl,-rpath-link,${PROJECT_BUILD_DIR}/lib ${LINK_RPATH_FLAG} -Wl,-rpath,'$ORIGIN/../plugins/${CMAKE_PROJECT_NAME}'"
                           LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
                endif()

                add_executable(${TEST_NAME}
                    ../../../tests/src/testsutils.cpp

                    ${ARG_SOURCES}
                    ${RESOURCES}

                    ${TEST_SOURCE}
                    ${TESTS_QRC_FILENAME}
                )

                set_target_properties(${TEST_NAME} PROPERTIES
                    OUTPUT_NAME ${TEST_NAME}
                    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
                )

                configure_clang_and_clang_tidy(${TEST_NAME})

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
                                ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/${ARG_EXTERNAL_BINARY}
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

                # Clean up our plugin's tests, if we are on macOS, or make sure
                # that it uses RPATH rather than RUNPATH on Linux

                if(APPLE)
                    strip_file(${TEST_NAME} ${DEST_TESTS_DIR}/${TEST_FILENAME})
                elseif(NOT WIN32)
                    runpath2rpath(${TEST_NAME} ${DEST_TESTS_DIR}/${TEST_FILENAME})
                endif()
            else()
                message(AUTHOR_WARNING "The '${ARG_TEST}' test for the '${PLUGIN_NAME}' plugin does not exist...")
            endif()
        endforeach()
    endif()
endmacro()

#===============================================================================

macro(copy_file_to_build_dir PROJECT_TARGET ORIG_DIR DEST_DIR FILENAME)
    # Copy the file (renaming it, if needed) to the destination folder
    # Note: DIRECT is used to copy a file that doesn't first need to be built.
    #       This means that we can then use execute_process() rather than
    #       add_custom_command(), and thus reduce the length of the
    #       PROJECT_TARGET command, something that can be useful on Windows
    #       since the command might otherwise end up being too long for Windows
    #       to handle...

    if("${ARGN}" STREQUAL "")
        set(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIR}/${FILENAME})
    else()
        # An argument was passed so use it to rename the file, which is to be
        # copied

        set(FULL_DEST_FILENAME ${PROJECT_BUILD_DIR}/${DEST_DIR}/${ARGN})
    endif()

    if("${PROJECT_TARGET}" STREQUAL "DIRECT")
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIR}/${FILENAME}
                                                         ${FULL_DEST_FILENAME})
    else()
        add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIR}/${FILENAME}
                                                            ${FULL_DEST_FILENAME}
                           BYPRODUCTS ${FULL_DEST_FILENAME})
    endif()
endmacro()

#===============================================================================

macro(windows_deploy_binary_file DIRNAME FILENAME)
    # Copy the binary file to both the build and build/bin folders, so we can
    # test things both from within Qt Creator and without first having to deploy
    # OpenCOR

    copy_file_to_build_dir(DIRECT ${DIRNAME} . ${FILENAME})
    copy_file_to_build_dir(DIRECT ${DIRNAME} bin ${FILENAME})

    # Deploy the binary file

    install(FILES ${DIRNAME}/${FILENAME}
            DESTINATION bin)
endmacro()

#===============================================================================

macro(windows_deploy_qt_library LIBRARY_NAME)
    # Deploy the Qt library

    if(   "${LIBRARY_NAME}" STREQUAL "WebKit"
       OR "${LIBRARY_NAME}" STREQUAL "WebKitWidgets")
        set(BINARIES_DIR ${QTWEBKIT_BINARIES_DIR})
    else()
        set(BINARIES_DIR ${QT_BINARIES_DIR})
    endif()

    windows_deploy_binary_file(${BINARIES_DIR} Qt5${LIBRARY_NAME}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
endmacro()

#===============================================================================

macro(windows_deploy_icu_library LIBRARY_NAME)
    # Deploy the ICU library

    windows_deploy_binary_file(${QTWEBKIT_BINARIES_DIR} icu${LIBRARY_NAME}${DEBUG_TAG}${ICU_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
endmacro()

#===============================================================================

macro(windows_deploy_qt_plugins PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        set(PLUGIN_ORIG_DIR ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        set(PLUGIN_DEST_DIR plugins/${PLUGIN_CATEGORY})
        set(PLUGIN_FILENAME ${PLUGIN_NAME}${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})

        copy_file_to_build_dir(DIRECT ${PLUGIN_ORIG_DIR} ${PLUGIN_DEST_DIR} ${PLUGIN_FILENAME})

        # Deploy the Qt plugin

        install(FILES ${PLUGIN_ORIG_DIR}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIR})
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

macro(linux_deploy_binary_file PROJECT_TARGET ORIG_DIR DEST_DIR FILENAME)
    # Copy the binary file to the build/lib folder, so we can test things
    # without first having to deploy OpenCOR
    # Note: this is particularly useful when the Linux machine has different
    #       versions of Qt...

    copy_file_to_build_dir(${PROJECT_TARGET} ${ORIG_DIR} ${DEST_DIR} ${FILENAME})

    # Make sure that the RUNPATH value is converted to an RPATH value

    runpath2rpath(${PROJECT_TARGET} ${PROJECT_BUILD_DIR}/${DEST_DIR}/${FILENAME})

    # Strip the binary file of all its local symbols

    strip_file(${PROJECT_TARGET} ${PROJECT_BUILD_DIR}/${DEST_DIR}/${FILENAME})

    # Deploy the binary file

    install(FILES ${PROJECT_BUILD_DIR}/${DEST_DIR}/${FILENAME}
            DESTINATION ${DEST_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro()

#===============================================================================

macro(linux_deploy_qt_library LIBRARY_NAME)
    # Deploy the Qt library

    if(   "${LIBRARY_NAME}" STREQUAL "${WEBKIT}"
       OR "${LIBRARY_NAME}" STREQUAL "${WEBKITWIDGETS}")
        set(ORIG_DIR ${QTWEBKIT_LIBRARIES_DIR})
    else()
        set(ORIG_DIR ${QT_LIBRARIES_DIR})
    endif()

    linux_deploy_binary_file(DIRECT ${ORIG_DIR} lib ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR})
endmacro()

#===============================================================================

macro(linux_deploy_qt_plugins PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Copy the Qt plugin to the plugins folder

        set(PLUGIN_ORIG_DIR ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY})
        set(PLUGIN_DEST_DIR plugins/${PLUGIN_CATEGORY})
        set(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        copy_file_to_build_dir(DIRECT ${PLUGIN_ORIG_DIR} ${PLUGIN_DEST_DIR} ${PLUGIN_FILENAME})

        # Make sure that the RUNPATH value is converted to an RPATH value

        runpath2rpath(DIRECT ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIR}/${PLUGIN_FILENAME})

        # Strip the Qt plugin of all its local symbols

        strip_file(DIRECT ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIR}/${PLUGIN_FILENAME})

        # Deploy the Qt plugin

        install(FILES ${PROJECT_BUILD_DIR}/${PLUGIN_DEST_DIR}/${PLUGIN_FILENAME}
                DESTINATION ${PLUGIN_DEST_DIR})
    endforeach()
endmacro()

#===============================================================================

macro(linux_deploy_system_library FILENAME NEW_FILENAME)
    # Copy the system library to the build/lib folder, so we can test things
    # without first having to deploy OpenCOR

    get_filename_component(REAL_FULL_FILENAME ${FILENAME} REALPATH)
    get_filename_component(REAL_DIR ${REAL_FULL_FILENAME} DIRECTORY)
    get_filename_component(REAL_FILENAME ${REAL_FULL_FILENAME} NAME)

    copy_file_to_build_dir(DIRECT ${REAL_DIR} lib ${REAL_FILENAME} ${NEW_FILENAME})

    # Deploy the system library

    install(FILES ${REAL_FULL_FILENAME}
            DESTINATION lib
            RENAME ${NEW_FILENAME})
endmacro()

#===============================================================================

macro(macos_deploy_qt_file ORIG_DIR DEST_DIR FILENAME)
    # Copy the Qt file

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIR}/${FILENAME}
                                                     ${DEST_DIR}/${FILENAME})

    # Clean up the Qt file

    strip_file(DIRECT ${DEST_DIR}/${FILENAME})
endmacro()

#===============================================================================

macro(macos_deploy_qt_library LIBRARY_NAME)
    # Deploy the Qt library

    set(QT_LIBRARY_NAME Qt${LIBRARY_NAME})
    set(QT_FRAMEWORK_DIR ${QT_LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR})

    if(   "${QT_LIBRARY_NAME}" STREQUAL "Qt${WEBKIT}"
       OR "${QT_LIBRARY_NAME}" STREQUAL "Qt${WEBKITWIDGETS}")
        set(REAL_QT_LIBRARIES_DIR ${QTWEBKIT_LIBRARIES_DIR})
    else()
        set(REAL_QT_LIBRARIES_DIR ${QT_LIBRARIES_DIR})
    endif()

    macos_deploy_qt_file(${REAL_QT_LIBRARIES_DIR}/${QT_FRAMEWORK_DIR}
                         ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks/${QT_FRAMEWORK_DIR}
                         ${QT_LIBRARY_NAME})
endmacro()

#===============================================================================

macro(macos_deploy_qt_plugins PLUGIN_CATEGORY)
    foreach(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin

        macos_deploy_qt_file(${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}
                             ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${PLUGIN_CATEGORY}
                             ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
    endforeach()
endmacro()

#===============================================================================

macro(create_package PACKAGE_NAME PACKAGE_VERSION PACKAGE_REPOSITORY RELEASE_TAG)
    # Various initialisations

    set(OPTIONS
        NO_STRIP
    )

    cmake_parse_arguments(ARG "${OPTIONS}" "" "" ${ARGN})

    set(PACKAGED_FILES ${ARGN})
    list(REMOVE_ITEM PACKAGED_FILES NO_STRIP)
    string(REPLACE ";" " "
           PACKAGED_FILES "${PACKAGED_FILES}")

    # The actual packaging code goes into a separate CMake script file that is
    # run as a POST_BUILD step

    set(PACKAGE_FILE ${PROJECT_BUILD_DIR}/${PACKAGE_NAME}.${PACKAGE_VERSION}.${TARGET_PLATFORM}.tar.gz)
    set(PACKAGING_SCRIPT ${PROJECT_BINARY_DIR}/package${PACKAGE_NAME}.cmake)
    set(CMAKE_CODE "cmake_minimum_required(VERSION 3.14)")

    if(NOT WIN32 AND RELEASE_MODE AND NOT ARG_NO_STRIP)
        set(CMAKE_CODE "${CMAKE_CODE}\n
# Strip our files

foreach(PACKAGED_FILENAME ${PACKAGED_FILES})
    execute_process(COMMAND strip -x \$\{PACKAGED_FILENAME\}
                    WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                    OUTPUT_QUIET ERROR_QUIET)
endforeach()")
    endif()

    set(CMAKE_CODE "${CMAKE_CODE}\n
# Compress our package

execute_process(COMMAND ${CMAKE_COMMAND} -E tar -czf ${PACKAGE_FILE} ${PACKAGED_FILES}
                WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                RESULT_VARIABLE RESULT
                OUTPUT_QUIET)

# Make sure that the package file exists

if(RESULT EQUAL 0 AND EXISTS ${PACKAGE_FILE})
    # The package file, so calculate its SHA-1 value and let people know how we
    # should call the retrieve_package() macro

    file(SHA1 ${PACKAGE_FILE} SHA1_VALUE)

    message(\"To retrieve the '${PACKAGE_NAME}' package, use:
retrieve_package(\\$\\{PACKAGE_NAME\\} \\$\\{PACKAGE_VERSION\\}
                 \\$\\{PACKAGE_REPOSITORY\\} \\$\\{RELEASE_TAG\\}
                 \$\{SHA1_VALUE\})\")
else()
    if(EXISTS ${PACKAGE_FILE})
        file(REMOVE ${PACKAGE_FILE})
    endif()

    message(FATAL_ERROR \"The '${PACKAGE_NAME}' package could not be created...\")
endif()
")

    file(WRITE ${PACKAGING_SCRIPT} ${CMAKE_CODE})

    # Run the packaging script once the dependency target has been satisfied

    add_custom_command(TARGET ${PACKAGE_NAME}Build POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -P ${PACKAGING_SCRIPT}
                       VERBATIM)
endmacro()

#===============================================================================

macro(get_full_local_external_package_dir)
    set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}/${PROJECT_NAME})

    if("${PLUGIN}" MATCHES "^thirdParty/.*$")
        string(REPLACE "Plugin" ""
               FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}")
    endif()
endmacro()

#===============================================================================

macro(retrieve_package PACKAGE_NAME PACKAGE_VERSION PACKAGE_REPOSITORY RELEASE_TAG SHA1_VALUE)
    # Check whether the package has already been retrieved by simply checking
    # whether its installation directory exists

    get_full_local_external_package_dir()

    message("Retrieving the '${PACKAGE_NAME}' package...")

    if(NOT EXISTS ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
        # Retrieve the package

        set(PACKAGE_FILE ${PACKAGE_NAME}.${PACKAGE_VERSION}.${TARGET_PLATFORM}.tar.gz)
        set(REAL_PACKAGE_FILE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PACKAGE_FILE})
        set(PACKAGE_URL "https://github.com/opencor/${PACKAGE_REPOSITORY}/releases/download/${RELEASE_TAG}/${PACKAGE_FILE}")

        file(DOWNLOAD ${PACKAGE_URL} ${REAL_PACKAGE_FILE}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the package, should we have managed to retrieve it

        list(GET STATUS 0 STATUS_CODE)

        if(${STATUS_CODE} EQUAL 0)
            file(SHA1 ${REAL_PACKAGE_FILE} REAL_SHA1_VALUE)

            if(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
                file(REMOVE_RECURSE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

                message(FATAL_ERROR "The '${PACKAGE_NAME}' package (downloaded from '${PACKAGE_URL}') does not have the expected SHA-1 value...")
            endif()

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${REAL_PACKAGE_FILE}
                            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                            RESULT_VARIABLE RESULT
                            OUTPUT_QUIET ERROR_QUIET)

            if(NOT RESULT EQUAL 0)
                file(REMOVE_RECURSE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

                message(FATAL_ERROR "The '${PACKAGE_NAME}' package (downloaded from '${PACKAGE_URL}') could not be uncompressed...")
            endif()

            file(REMOVE ${REAL_PACKAGE_FILE})
        else()
            file(REMOVE_RECURSE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

            message(FATAL_ERROR "The '${PACKAGE_NAME}' package could not be retrieved from '${PACKAGE_URL}'...")
        endif()
    endif()
endmacro()
