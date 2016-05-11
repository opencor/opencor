PROJECT(SBMLAPIPlugin)

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    include/sbml/common/PLATFORM_DIR/libsbml-config-common.h
)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 3a24a745b8243bef47a8cacec659a4e0ece26e80)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 475d64f9c36001f00506e21ae0eb218e329d2f27)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll d34fd9be2a60c6e44514feef667ae7eb95a561cd)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 6632710313fc56519012f69154e29c75ef67c93c)
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.5.dylib 091de6fde58f9ea65ad4ae041187577ffd0bf6d1)
ELSE()
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.so.5 474e10fa04b4a41467cb3e43716c7ecfbb515e8f)
ENDIF()

# Retrieve the SBML API external binaries

SET(EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

IF(WIN32)
    SET(EXTERNAL_BINARIES
        libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX}
    )
ELSEIF(APPLE)
    SET(EXTERNAL_BINARIES
        libsbml.5${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    SET(EXTERNAL_BINARIES
        libsbml${CMAKE_SHARED_LIBRARY_SUFFIX}.5
    )
ENDIF()

SET(SBML_API_EXTERNAL_BINARIES ${EXTERNAL_BINARIES} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(SBMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/sbmlapiplugin.cpp
    HEADERS_MOC
        src/sbmlapiplugin.h
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
                           libsbml${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} sbml.5)
ELSE()
    FOREACH(EXTERNAL_BINARY ${EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} ${EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
