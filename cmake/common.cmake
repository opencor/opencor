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

MACRO(INITIALISE_THIRD_PARTY_LIBRARY)
    SET(SOURCES)
    SET(HEADERS_MOC)
ENDMACRO()

MACRO(INITIALISE_PLUGIN INTERNATIONALISATION)
    SET(SOURCES)
    SET(HEADERS)
    SET(HEADERS_MOC)
    SET(UIS)

    IF(${INTERNATIONALISATION})
        SET(RESOURCES
            res/${PROJECT_NAME}.qrc
        )
    ELSE()
        SET(RESOURCES)
    ENDIF()
ENDMACRO()

MACRO(UPDATE_LANGUAGE_FILES)
    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the project itself
    # Note: this requires SOURCES, HEADERS, HEADERS_MOC and UIS to be defined
    #       for the current CMake project, even if that means that these
    #       variables are to be empty (the case with some plugins for example).
    #       Indeed, since otherwise the value of these variables, as defined in
    #       a previous project, may be used, so...

    SET(LANGUAGE_FILES
        ${PROJECT_NAME}_fr
    )

    FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

        EXECUTE_PROCESS(COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete
                                                         ${SOURCES} ${HEADERS} ${HEADERS_MOC} ${UIS}
                                                     -ts ${TS_FILE}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS_FILE}
                                                      -qm ${CMAKE_SOURCE_DIR}/build/${LANGUAGE_FILE}.qm
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    ENDFOREACH()
ENDMACRO()

MACRO(BUILD_THIRD_PARTY_LIBRARY THIRD_PARTY_LIBRARY_NAME)
    # Rules to build the third-party library

    IF("${HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    ENDIF()

    ADD_LIBRARY(${THIRD_PARTY_LIBRARY_NAME} SHARED
        ${SOURCES}
        ${SOURCES_MOC}
    )

    TARGET_LINK_LIBRARIES(${THIRD_PARTY_LIBRARY_NAME}
        ${ARGN}
    )

    # Linker settings

    SET_TARGET_PROPERTIES(${THIRD_PARTY_LIBRARY_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")
ENDMACRO()

MACRO(BUILD_PLUGIN PLUGIN_NAME)
    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the plugin itself

    IF(NOT "${RESOURCES}" STREQUAL "")
        UPDATE_LANGUAGE_FILES()
    ENDIF()

    # Rules to build the plugin

    QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    QT4_WRAP_UI(SOURCES_UIS ${UIS})

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
    ENDIF()

    ADD_LIBRARY(${PLUGIN_NAME} SHARED
        ${SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
        ${ARGN}
    )

    # Linker settings

    SET_TARGET_PROPERTIES(${PLUGIN_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")
ENDMACRO()

MACRO(PACKAGE_THIRD_PARTY_LIBRARY)
    IF(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    ELSEIF(NOT APPLE)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
    ENDIF()
ENDMACRO()

MACRO(PACKAGE_PLUGIN)
    #---GRY---

    # Deployed location:
    # ------------------
    # The plugin is currently deployed in the same location as third-party
    # libraries, since OpenCOR doesn't currently support plugins as such.
    # However, once it does, the plugin will have to be deployed in its correct
    # location, i.e. the "plugins" folder. For the time being, though, the
    # current approach allows to test things, so...

    # Translation files:
    # ------------------
    # There is currently no 'proper' support for the internationalisation of
    # the plugin. This is therefore something that will have to be added at
    # some point.

    IF(WIN32)
#        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION plugins)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    ELSEIF(NOT APPLE)
#        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
    ENDIF()

MESSAGE("")
MESSAGE("********* WARNING **********")
MESSAGE("Call to PACKAGE_PLUGIN (in [OpenCOR]/cmake/common.cmake) for the ${PROJECT_NAME} plugin: please note that the plugin is not currently being deployed in the right location. Also, there is currently no 'proper' support for the internationalisation of the plugin.")
MESSAGE("****************************")
MESSAGE("")
ENDMACRO()
