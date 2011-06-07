MACRO(INITIALISATION)
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
    # There is currently no support for the internationalisation of the plugin.
    # This is therefore something that will have to be added at some point.

    IF(WIN32)
#        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION plugins)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    ELSEIF(NOT APPLE)
#        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
    ENDIF()

MESSAGE(WARNING "Call to PACKAGE_PLUGIN for ${PROJECT_NAME}: please note that the plugin is not currently being deployed in the right location. Also, there is currently no support for the internationalisation of the plugin.")
ENDMACRO()
