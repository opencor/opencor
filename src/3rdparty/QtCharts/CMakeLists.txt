PROJECT(QtCharts)

# Name and version of our package

SET(PACKAGE_NAME QtCharts)
SET(PACKAGE_VERSION 5.9.0)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 5)
SET(MINOR_LIBRARY_VERSION 9)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tag for our library

SET(GIT_TAG 5.9.0)

# Platform target for our library

SET(TARGET_PLATFORM ${PLATFORM})

# Specify where our local package will be installed
# Note: on Windows, we would normally distinguish between release and debug
#       modes, but QtCharts' build system doesn't, hence we go one folder up...

IF(WIN32)
    STRING(REGEX REPLACE "/[^/]+$" "" REAL_LOCAL_EXTERNAL_PACKAGE_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR})
ELSE()
    SET(REAL_LOCAL_EXTERNAL_PACKAGE_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR})
ENDIF()

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${REAL_LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(RELEASE_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}Charts${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(RELEASE_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}Charts${CMAKE_IMPORT_LIBRARY_SUFFIX})
    SET(DEBUG_SHARED_LIBRARY Qt${QT_VERSION_MAJOR}Chartsd${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(DEBUG_IMPORT_LIBRARY Qt${QT_VERSION_MAJOR}Chartsd${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(QTCHARTS_DIR QtCharts.framework)
    SET(SHARED_LIBRARY_DIR ${QTCHARTS_DIR}/Versions/${QT_VERSION_MAJOR})
    SET(SHARED_LIBRARY ${SHARED_LIBRARY_DIR}/QtCharts)
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ELSE()
    SET(BASE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}Charts${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SHARED_LIBRARY ${BASE_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION})
    SET(REAL_SHARED_LIBRARY ${BASE_SHARED_LIBRARY}.${LIBRARY_VERSION})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Specify the files that need to have their SHA-1 value checked

IF(WIN32)
    SET(SHA1_FILES
        bin/${RELEASE_SHARED_LIBRARY}
        bin/${DEBUG_SHARED_LIBRARY}

        lib/${RELEASE_IMPORT_LIBRARY}
        lib/${DEBUG_IMPORT_LIBRARY}
    )
ELSE()
    SET(SHA1_FILES
        lib/${SHARED_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QTCHARTS_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${PLATFORM_DIR}")

    IF(WIN32)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} d7290d479c488964e43088bdc0c855494fa105ab
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 9d73b40c8528472746988f7e6bb8c923f2972d10
                                          336e7ff0119f0b26ecbb0208e3f014933e8c6ca6
                                          6258729d778866c63eea221eb96428eb81c855f6
                                          87b1ba707145d080fa90261358739179aad4e435)
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1b570cf296aefc49f8e742b234e3cfaa5f8b3ac7
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c819db40834569161082e4d95ef78efcbfe5811b)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 9f7c14a13d42f5ecc34c1a4663e9484300dc1382
                              TARGET_PLATFORM ${TARGET_PLATFORM}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 91a78fef36643c0c499c64f94960cdeb3fd22731)
    ENDIF()
ELSE()
    # Set platform specific configuration options

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    SET(QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR ${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD})

    IF(WIN32)
        SET(BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${RELEASE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${DEBUG_IMPORT_LIBRARY}
        )
    ELSE()
        SET(BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
        )
    ENDIF()

    IF(WIN32)
        SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} CONFIG+=debug_and_release CONFIG+=build_all <SOURCE_DIR>)
        SET(BUILD_COMMAND ${MAKE_JOM_COMMAND})
        SET(INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
            && ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/include <INSTALL_DIR>/include
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${RELEASE_SHARED_LIBRARY} <INSTALL_DIR>/bin/${RELEASE_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${RELEASE_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${RELEASE_IMPORT_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${DEBUG_SHARED_LIBRARY} <INSTALL_DIR>/bin/${DEBUG_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${DEBUG_IMPORT_LIBRARY} <INSTALL_DIR>/lib/${DEBUG_IMPORT_LIBRARY}
        )
    ELSEIF(APPLE)
        IF(RELEASE_MODE)
            SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} CONFIG+=release <SOURCE_DIR>)
        ELSE()
            SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} CONFIG+=debug <SOURCE_DIR>)
        ENDIF()

        SET(BUILD_COMMAND ${MAKE_JOM_COMMAND})
        SET(INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
            && ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${SHARED_LIBRARY_DIR}/Headers <INSTALL_DIR>/lib/${SHARED_LIBRARY_DIR}/Headers
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${SHARED_LIBRARY} <INSTALL_DIR>/lib/${SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E create_symlink 5 <INSTALL_DIR>/lib/${QTCHARTS_DIR}/Versions/Current
            && ${CMAKE_COMMAND} -E create_symlink Versions/Current/Headers <INSTALL_DIR>/lib/${QTCHARTS_DIR}/Headers
            && ${CMAKE_COMMAND} -E create_symlink Versions/Current/QtCharts <INSTALL_DIR>/lib/${QTCHARTS_DIR}/QtCharts
        )
    ELSE()
        IF(RELEASE_MODE)
            SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} CONFIG+=release <SOURCE_DIR>)
        ELSE()
            SET(CONFIGURE_COMMAND ${QMAKE_COMMAND} CONFIG+=debug <SOURCE_DIR>)
        ENDIF()

        SET(BUILD_COMMAND ${MAKE_JOM_COMMAND})
        SET(INSTALL_COMMAND
               ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/cmake <INSTALL_DIR>/cmake
            && ${CMAKE_COMMAND} -E copy_directory ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/include <INSTALL_DIR>/include
            && ${CMAKE_COMMAND} -E copy ${QTCHARTS_EXTERNAL_PROJECT_BUILD_DIR}/lib/${REAL_SHARED_LIBRARY} <INSTALL_DIR>/lib/${REAL_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E create_symlink ${REAL_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_SHARED_LIBRARY}
            && ${CMAKE_COMMAND} -E create_symlink ${REAL_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}
            && ${CMAKE_COMMAND} -E create_symlink ${REAL_SHARED_LIBRARY} <INSTALL_DIR>/lib/${BASE_SHARED_LIBRARY}.${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}
        )
    ENDIF()

    # Build Qt Charts as an external project

    SET(SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${SOURCE_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qtcharts
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${CONFIGURE_COMMAND}
        BUILD_COMMAND
            ${BUILD_COMMAND}
        INSTALL_COMMAND
            ${INSTALL_COMMAND}
        BUILD_BYPRODUCTS
            ${BUILD_BYPRODUCTS}
    )

    # (Try to) build our patch CMake program

	BUILD_PATCHCMAKE()

    # Patch our CMake files

    ExternalProject_Add_Step(${PACKAGE_BUILD} PatchCMakeFiles
                             COMMAND ${PATCH} Qt5Charts/Qt5ChartsConfig.cmake Charts
                             WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/cmake
                             DEPENDEES install)

    # Make sure that we have the actual contents of our header files
    # Note: this is only for Windows and Linux since, on those platforms, some
    #       of our 'installed' header files are indeed #including our copy of
    #       the Qt Charts header files rather than having their actual
    #       contents...

    IF(WIN32 OR NOT APPLE)
        ExternalProject_Add_Step(${PACKAGE_BUILD} PatchHeaderFiles
                                 COMMAND ${CMAKE_COMMAND} -D SOURCE_DIR=${SOURCE_DIR}
                                                          -D INCLUDE_DIR=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include
                                                          -P ${CMAKE_SOURCE_DIR}/cmake/patchheaderfiles.cmake
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${REAL_LOCAL_EXTERNAL_PACKAGE_DIR}
                        TARGET_PLATFORM ${TARGET_PLATFORM}
                        PACKAGED_FILES .
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})
ENDIF()

# Make sure that Qt Charts and its CMake files can be found

SET(QTCHARTS_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtCharts)

SET(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${QTCHARTS_DIR}/ext/cmake/Qt5Charts
    PARENT_SCOPE
)

SET(QTCHARTS_BINARIES_DIR ${QTCHARTS_DIR}/ext/bin PARENT_SCOPE)
SET(QTCHARTS_LIBRARIES_DIR ${QTCHARTS_DIR}/ext/lib PARENT_SCOPE)
