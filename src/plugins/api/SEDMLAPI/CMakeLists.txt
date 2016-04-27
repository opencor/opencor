PROJECT(SEDMLAPIPlugin)

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    include/sedml/common/PLATFORM_DIR/libsedml-config-common.h
)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll 79e22e9e7f7db6a2a906a8f291c74e40c072a206)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib eff9e7652f873ab15ac4f5295fc90a49965c791f)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll 70fc3e9f68e0e3efcd6e0aa52d5a3f879a799b44)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib 7c74705cac33c83a602388eaaa516cfe591d6d5b)
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.0.dylib 97264b848800ec0f3c9624fbdc8370aef2e13ee4)
ELSE()
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.so.0 5d1cb6f0bea2a3fed6aee624de3cd0bbc17860eb)
ENDIF()

# Retrieve the SED-ML API external binaries

SET(EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

IF(WIN32)
    SET(EXTERNAL_BINARIES
        libsedml${CMAKE_IMPORT_LIBRARY_SUFFIX}
    )
ELSEIF(APPLE)
    SET(EXTERNAL_BINARIES
        libsedml.0${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    SET(EXTERNAL_BINARIES
        libsedml${CMAKE_SHARED_LIBRARY_SUFFIX}.0
    )
ENDIF()

SET(SEDML_API_EXTERNAL_BINARIES ${EXTERNAL_BINARIES} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(SEDMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/sedmlapiplugin.cpp
    HEADERS_MOC
        src/sedmlapiplugin.h
    INCLUDE_DIRS
        include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    TESTS
        tests
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR}
                           libsedml${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} sedml.0
                        sbml.5)
ELSE()
    FOREACH(EXTERNAL_BINARY ${EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} ${EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
