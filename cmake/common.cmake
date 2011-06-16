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
