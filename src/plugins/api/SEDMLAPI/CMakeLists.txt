PROJECT(SEDMLAPIPlugin)

MACRO(RETRIEVE_SEDML_API_SETTINGS)
    # Retrieve some SED-ML API settings

    IF(WIN32)
        SET(SEDML_API_EXTERNAL_BINARIES
            libsedml${CMAKE_IMPORT_LIBRARY_SUFFIX}
        )
    ELSEIF(APPLE)
        SET(SEDML_API_EXTERNAL_BINARIES
            libsedml.0${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
    ELSE()
        SET(SEDML_API_EXTERNAL_BINARIES
            libsedml${CMAKE_SHARED_LIBRARY_SUFFIX}.0
        )
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    include/sedml/common/DISTRIB_DIR/libsedml-config-common.h
)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

IF(WIN32)
    IF(${ARCHITECTURE} EQUAL 32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll bd7425efbb620ded81120558f601a236d729cfcb)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib a74b08f75f1b07b49f524711ae8ff5f360ac7058)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll 9bbd764106dc912a044a1b9abe691d7c65c51b97)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib 4c2692fc5727c73075c8d33c6329947b5d8b7db7)
        ENDIF()
    ELSE()
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll f36e427c75d4b5ff60fa5efd2d321e3d1447c904)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib 213e7654b29c7c29587db044f975bfd9a7d1473d)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.dll fc389dc8fadf27fc42bbd1f790b262584c84254b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.lib 0da899cdef0b9ea6f485de2ee0481ba9d3aae4b3)
        ENDIF()
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.0.dylib f8a7b597b27df86fb0eab5ed81b93b27a5453123)
ELSE()
    IF(${ARCHITECTURE} EQUAL 32)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.so.0 e52809ae2f688663875ebae9f4299ac6d32f6ca2)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libsedml.so.0 0e65b32e931830962edcda4aedd8105a61eb83a2)
    ENDIF()
ENDIF()

# Retrieve some SED-ML API settings

RETRIEVE_SEDML_API_SETTINGS()

# Add the plugin

SET(SEDML_API_EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

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
    QT_LIBRARIES
        QtCore
    EXTERNAL_BINARIES_DIR
        ${SEDML_API_EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${SEDML_API_EXTERNAL_BINARIES}
    TESTS
        tests
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${SEDML_API_EXTERNAL_BINARIES_DIR}
                           libsedml${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${SEDML_API_EXTERNAL_BINARIES_DIR} sedml.0
                        sbml.5)
ELSE()
    FOREACH(SEDML_API_EXTERNAL_BINARY ${SEDML_API_EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${SEDML_API_EXTERNAL_BINARIES_DIR}
                             ${SEDML_API_EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
