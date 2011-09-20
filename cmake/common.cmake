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
            # Note #1: -Wl,-s strips all the symbols, thus reducing the final size
            #          of OpenCOR of one its shared libraries
            # Note #2: the above linking option has become obsolete on Mac OS X,
            #          so...
        ENDIF()
    ENDIF()

    # Required packages

    IF(APPLE)
        # Note #1: the Qt SDK doesn't, by default, make the Qt binaries
        #          available to the user, hence we must update the user's PATH.
        #          However, this doesn't work on Mac OS X with Qt Creator, so
        #          we must hard-code qmake's path so that CMake can find it
        #          from within Qt Creator...
        # Note #2: ideally, we would use EXECUTE_PROCESS to retrieve the result
        #          of 'which qmake', but that CMake command doesn't work from
        #          within Qt Creator, so we really have no choice but to hard-
        #          code qmake's path...

        SET(QT_QMAKE_EXECUTABLE /Developer/QtSDK/Desktop/Qt/474/gcc/bin/qmake)
    ENDIF()

    FIND_PACKAGE(Qt4 4.7.4 REQUIRED)

    # Whether we are building for 32-bit or 64-bit

    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(64BIT_MODE ON)
    ELSE()
        SET(64BIT_MODE OFF)
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

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(SOURCES)
    SET(HEADERS)
    SET(HEADERS_MOC)
    SET(UIS)
    SET(PLUGIN_INCLUDE_DIRS)
    SET(DEFINITIONS)
    SET(OPENCOR_DEPENDENCIES)
    SET(QT_DEPENDENCIES)
    SET(EXTERNAL_DEPENDENCIES_DIR)
    SET(EXTERNAL_DEPENDENCIES)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "SOURCES")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS_MOC")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "UIS")
            SET(TYPE_OF_PARAMETER 4)
        ELSEIF(${PARAMETER} STREQUAL "PLUGIN_INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 5)
        ELSEIF(${PARAMETER} STREQUAL "DEFINITIONS")
            SET(TYPE_OF_PARAMETER 6)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 8)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES_DIR")
            SET(TYPE_OF_PARAMETER 9)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 10)
        ELSEIF(${PARAMETER} STREQUAL "RESOURCE_DIR")
            SET(TYPE_OF_PARAMETER 11)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

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
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 9)
                SET(EXTERNAL_DEPENDENCIES_DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 10)
                SET(EXTERNAL_DEPENDENCIES ${EXTERNAL_DEPENDENCIES} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 11)
                SET(QRC_FILE ${PARAMETER}/${PLUGIN_NAME}.qrc)
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Resource file, if any

    IF("${QRC_FILE}" STREQUAL "")
        SET(QRC_FILE res/${PLUGIN_NAME}.qrc)
    ENDIF()

    IF(EXISTS "${PROJECT_SOURCE_DIR}/${QRC_FILE}")
        SET(RESOURCES
            ${QRC_FILE}
        )
    ELSE()
        SET(RESOURCES)
    ENDIF()

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

    # Make sure that we link to our external dependencies

    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
        )
    ENDFOREACH()

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
    # Note: this is so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    IF(APPLE)
        SET(ORIG_PLUGINS_DIR ${CMAKE_BINARY_DIR}/lib)
        SET(DEST_PLUGINS_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME})
    ELSE()
        SET(ORIG_PLUGINS_DIR ${CMAKE_BINARY_DIR})
        SET(DEST_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    IF(NOT EXISTS ${DEST_PLUGINS_DIR})
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_PLUGINS_DIR})
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_PLUGINS_DIR}/${PLUGIN_FILENAME} ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

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
                               COMMAND install_name_tool -change ${MAC_OS_X_PROJECT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
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

        # Make sure that the plugin refers to our embedded version the external
        # dependencies on which it depends

        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself

    IF(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION plugins/${MAIN_PROJECT_NAME})
    ELSEIF(NOT APPLE)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins/${MAIN_PROJECT_NAME})
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

    # Create the folder hierarchy for the library/framework

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

    # Copy the library itself

    IF("${TYPE}" STREQUAL "Library")
        IF("${DIR}" STREQUAL "")
            SET(LIBRARY_LIB_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${QT_LIBRARY_DIR}/${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX} ${LIBRARY_LIB_FILEPATH})
        ELSE()
            SET(LIBRARY_LIB_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${DIR}/${LIBRARY_NAME} ${LIBRARY_LIB_FILEPATH})
        ENDIF()
    ELSE()
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
        # Make sure that the library refers to our embedded version of the
        # libraries on which it depends

        FOREACH(LIBRARY ${LIBRARIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}
                                                                 @executable_path/../Frameworks/${LIBRARY}
                                                                 ${LIBRARY_LIB_FILEPATH})
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT PLUGIN_DIRNAME PLUGIN_NAME)
    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND install_name_tool -id ${PLUGIN_FILENAME}
                                                     ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${PLUGIN_DIRNAME}/${PLUGIN_FILENAME})
ENDMACRO()
