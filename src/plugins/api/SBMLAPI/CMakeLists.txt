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
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 888ac2a300796235f169da857fb31670e707420b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 800eb94202647c8c1245efcf698c76bec5db41a7)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll 02fd4e51d562fb4bc02db024dbedf6e5806645b8)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib aa93ee2add4cad1e53fc3f66ccb773bd3ab0598e)
        ENDIF()
    ELSE()
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll e2fb73caa0cb20d6722a66638ce2a26233dc875f)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib eab202a68ddcb5ab0391ed149a5423d9222ffd42)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.dll db2df210af341e833fb79021ddda4ce1b8b31d0b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.lib 35d17fda6341331e1fb532f7b0742fd4ea94fe72)
        ENDIF()
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.5.dylib 56bd96fed5552cc946f5055fe1b006aae60fbe71)
ELSE()
    IF(${ARCHITECTURE} EQUAL 32)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.so.5 58938c4bba3e0ce69fe3fdb37a1e22b0f2c990be)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsbml.so.5 95861c1b47b81b0693dbf0fdddc976518828f586)
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
