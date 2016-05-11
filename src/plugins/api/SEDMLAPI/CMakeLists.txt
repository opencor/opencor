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
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll 5d89306f442fc9368c6da5e6bfcdb6ebcc0acd84)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib a307ca54c05d2c8cb186bfe5405ec0a9568f0ebf)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll 022819e52326d0712b0ca4053d71255ae6425980)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib 92fd72a183deeb3a2cb829571c6c182a1d29fe4b)
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.0.dylib 779c63627f34a8241fccc21ca861a94a35c5ab8e)
ELSE()
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.so.0 f25318c3523a82d331e30c16d934e19aa556bd95)
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
