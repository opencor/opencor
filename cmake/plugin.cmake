INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

MACRO(INITIALISE_PLUGIN TARGET_NAME HAS_RESOURCES)
    SET(PLUGIN_NAME ${TARGET_NAME})

    SET(SOURCES)
    SET(HEADERS)
    SET(HEADERS_MOC)
    SET(UIS)

    IF(${HAS_RESOURCES})
        SET(RESOURCES
            res/${PLUGIN_NAME}.qrc
        )
    ELSE()
        SET(RESOURCES)
    ENDIF()
ENDMACRO()

MACRO(BUILD_PLUGIN)
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

    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${ARGN}
    )

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
ENDMACRO()

MACRO(SET_PLUGIN_QT_LIBRARIES)
    # Make sure that the plugin refers to our embedded version of the Qt
    # libraries which it needs

    IF(APPLE)
        FOREACH(LIBRARY ${ARGN})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                                 @executable_path/../Frameworks/${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(PACKAGE_PLUGIN)
    IF(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin/plugins)
    ELSEIF(NOT APPLE)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
    ENDIF()
ENDMACRO()
