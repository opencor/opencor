PROJECT(SBMLAPIPlugin)

MACRO(RETRIEVE_SBML_API_SETTINGS)
    # Retrieve some SBML API settings

    IF(WIN32)
        SET(SBML_API_EXTERNAL_BINARIES
            libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX}
        )
    ELSEIF(APPLE)
        SET(SBML_API_EXTERNAL_BINARIES
            libsbml.5${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
    ELSE()
        SET(SBML_API_EXTERNAL_BINARIES
            libsbml${CMAKE_SHARED_LIBRARY_SUFFIX}.5
        )
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    include/sbml/common/DISTRIB_DIR/libsbml-config-common.h
)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

IF(WIN32)
    IF(${ARCHITECTURE} EQUAL 32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 601167f2d6d2313fed62c790c5dcf5d970562f8e)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 7f744699dfcb65a62db3ddddfa6dbcd0c8a7770c)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 3bf284697c731f1f13a4be6c8da703bee6a99df4)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib e35489cea2df3f5dd2c1bba42bc43ace3baad31b)
        ENDIF()
    ELSE()
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 747baf0cac9e7f26fc0c5bf155a6a16ab1eb4bbb)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 76b8a9cf2387f19c0a4133baa044d71c3a1b25af)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 188844c5931dbb5f645a200fa595ad530c90344a)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib a215856cd4042c372c8616b7df19fc789d0cb330)
        ENDIF()
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.5.dylib bd814fe1371db1c953fcae0b99588bae099205aa)
ELSE()
    IF(${ARCHITECTURE} EQUAL 32)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.so.5 3fad1c869afad8bdd9119b3682c290886b5275fa)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.so.5 9c20b9535e32209728884d8f1d48b79b59aaef20)
    ENDIF()
ENDIF()

# Retrieve some SBML API settings

RETRIEVE_SBML_API_SETTINGS()

# Add the plugin

SET(SBML_API_EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

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
        ${SBML_API_EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${SBML_API_EXTERNAL_BINARIES}
    TESTS
        tests
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${SBML_API_EXTERNAL_BINARIES_DIR}
                           libsbml${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${SBML_API_EXTERNAL_BINARIES_DIR} sbml.5)
ELSE()
    FOREACH(SBML_API_EXTERNAL_BINARY ${SBML_API_EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${SBML_API_EXTERNAL_BINARIES_DIR}
                             ${SBML_API_EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
