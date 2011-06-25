MACRO(INITIALISE_PROJECT)
#    SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Required packages

    FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

    # Type of build
    # Note: we do this here, so that all of our third-party libraries and
    #       plugins can benefit from these settings

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        SET(DEBUG_MODE ON)

        MESSAGE("Building a debug version...")
    ELSE()
        SET(DEBUG_MODE OFF)

        MESSAGE("Building a release version...")
    ENDIF()

    # Default compiler and linker settings

    IF(${DEBUG_MODE})
        SET(CMAKE_CXX_FLAGS "-g -O0")
    ELSE()
        SET(CMAKE_CXX_FLAGS "-O2 -ffast-math")
    ENDIF()

    SET(LINK_FLAGS_PROPERTIES)

    IF(WIN32)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-enable-auto-import")
        # Note: -Wl,-enable-auto-import allows us to use our various
        #       third-party libraries without having to worry about importing
        #       their functions, etc.
    ENDIF()

    IF(NOT APPLE AND NOT ${DEBUG_MODE})
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
        # Note #1: -Wl,-s strips all the symbols, thus reducing the final size
        #          of OpenCOR of one its shared libraries
        # Note #2: the above linking option has become obsolete on Mac OS X,
        #          so...
    ENDIF()

    # Default location for third-party libraries
    # Note: this is only required so that we can quickly test third-party
    #       libraries without first having to package everything

    IF(APPLE)
        SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
    ELSE()
        SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    ENDIF()
ENDMACRO()

MACRO(UPDATE_LANGUAGE_FILES TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files which will later on be embedded in the project
    # itself
    # Note: this requires SOURCES, HEADERS, HEADERS_MOC and UIS to be defined
    #       for the current CMake project, even if that means that these
    #       variables are to be empty (the case with some plugins for example).
    #       Indeed, since otherwise the value of these variables, as defined in
    #       a previous project, may be used, so...

    SET(LANGUAGE_FILES
        ${TARGET_NAME}_fr
    )

    FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

        IF(EXISTS "${PROJECT_SOURCE_DIR}/${TS_FILE}")
            EXECUTE_PROCESS(COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete
                                                             ${SOURCES} ${HEADERS} ${HEADERS_MOC} ${UIS}
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

MACRO(ADD_PLUGIN PLUGIN_NAME HAS_RESOURCES)
    # Initialise the plugin

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(SOURCES)
    SET(HEADERS)
    SET(HEADERS_MOC)
    SET(UIS)
    SET(PLUGIN_INCLUDE_DIRS)
    SET(DEFINITIONS)
    SET(OPENCOR_DEPENDENCIES)
    SET(QT_DEPENDENCIES)

    IF(${HAS_RESOURCES})
        SET(RESOURCES
            res/${PLUGIN_NAME}.qrc
        )
    ELSE()
        SET(RESOURCES)
    ENDIF()

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "SOURCES")
            # Switch to the SOURCE type of parameters

            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS")
            # Switch to the HEADERS type of parameters

            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS_MOC")
            # Switch to the HEADERS_MOC type of parameters

            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "UIS")
            # Switch to the UIS type of parameters

            SET(TYPE_OF_PARAMETER 4)
        ELSEIF(${PARAMETER} STREQUAL "PLUGIN_INCLUDE_DIRS")
            # Switch to the PLUGIN_INCLUDE_DIRS type of parameters

            SET(TYPE_OF_PARAMETER 5)
        ELSEIF(${PARAMETER} STREQUAL "DEFINITIONS")
            # Switch to the DEFINITIONS type of parameters

            SET(TYPE_OF_PARAMETER 6)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_DEPENDENCIES")
            # Switch to the OPENCOR_DEPENDENCIES type of parameters

            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            # Switch to the QT_DEPENDENCIES type of parameters

            SET(TYPE_OF_PARAMETER 8)
        ELSE()
            # Add the parameter to the corresponding set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                SET(SOURCES ${SOURCES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                SET(HEADERS ${HEADERS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                SET(HEADERS_MOC ${HEADERS_MOC} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                SET(UIS ${UIS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 5)
                SET(PLUGIN_INCLUDE_DIRS ${PLUGIN_INCLUDE_DIRS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 6)
                SET(DEFINITIONS ${DEFINITIONS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 7)
                SET(OPENCOR_DEPENDENCIES ${OPENCOR_DEPENDENCIES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 8)
                SET(QT_DEPENDENCIES ${QT_DEPENDENCIES} ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${PLUGIN_INCLUDE_DIRS})

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
    
    FOREACH(PLUGIN ${OPENCOR_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${PLUGIN}Plugin
        )
    ENDFOREACH()

    # Qt dependencies
    
    FOREACH(QT_LIBRARY ${QT_DEPENDENCIES})
        IF(WIN32)
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${QT_LIBRARY}${QT_VERSION_MAJOR}${CMAKE_STATIC_LIBRARY_SUFFIX})
        ELSEIF(APPLE)
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${QT_LIBRARY}.framework)
        ELSE()
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDIF()

        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${QT_LIBRARY_PATH}
        )
    ENDFOREACH()

    # Linker settings
    # Note: by default "lib" will be prepended to the name of the target file.
    #       However, this is not common practice on Windows, so...

    IF(WIN32)
        SET(CMAKE_SHARED_LIBRARY_PREFIX)
    ENDIF()

    SET_TARGET_PROPERTIES(${PROJECT_NAME}
        PROPERTIES OUTPUT_NAME ${PLUGIN_NAME} LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # Create the plugins directory if it doesn't already exist and move the
    # plugin to it
    # Note: this is so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    IF(APPLE)
        SET(ORIG_PLUGINS_DIR ${CMAKE_BINARY_DIR}/lib)
        SET(DEST_PLUGINS_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME})
    ELSE()
        SET(ORIG_PLUGINS_DIR ${CMAKE_BINARY_DIR})
        SET(DEST_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins)
    ENDIF()

    IF(NOT EXISTS ${PLUGINS_DIR})
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_PLUGINS_DIR})
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_PLUGINS_DIR}/${PLUGIN_FILENAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # Add some definitions

    FOREACH(DEFINITION ${DEFINITIONS})
        ADD_DEFINITIONS(-D${DEFINITION})
    ENDFOREACH()

    # Make sure that the plugin refers to our embedded version of itself and
    # other plugins on which it depends

    IF(APPLE)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -id @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                         ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        FOREACH(PLUGIN ${OPENCOR_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -id @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                             ${MAC_OS_X_PROJECT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()

    # Make sure that the plugin refers to our embedded version of the Qt
    # libraries on which it depends

    IF(APPLE)
        FOREACH(LIBRARY ${QT_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                                 @executable_path/../Frameworks/${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()

    # Package the plugin

    IF(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin/plugins)
    ELSEIF(NOT APPLE)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
    ENDIF()
ENDMACRO()
