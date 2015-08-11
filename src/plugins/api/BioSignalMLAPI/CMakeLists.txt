PROJECT(BioSignalMLAPIPlugin)

MACRO(RETRIEVE_BioSignalML_API_SETTINGS)
    # Retrieve some BioSignalML API settings

    IF(WIN32)
        SET(BioSignalML_API_EXTERNAL_BINARIES
            libbiosignalml${CMAKE_IMPORT_LIBRARY_SUFFIX}
            libtypedobject${CMAKE_IMPORT_LIBRARY_SUFFIX}
        )
    ELSEIF(APPLE)
        SET(BioSignalML_API_EXTERNAL_BINARIES
            libbiosignalml.0${CMAKE_SHARED_LIBRARY_SUFFIX}
            libtypedobject.1${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
    ELSE()
        SET(BioSignalML_API_EXTERNAL_BINARIES
            libbiosignalml${CMAKE_SHARED_LIBRARY_SUFFIX}.0
            libtypedobject${CMAKE_SHARED_LIBRARY_SUFFIX}.1
        )
    ENDIF()
ENDMACRO()

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

IF(WIN32)
    IF(${ARCHITECTURE} EQUAL 32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.dll 888ac2a300796235f169da857fb31670e707420b)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.lib 800eb94202647c8c1245efcf698c76bec5db41a7)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.dll 888ac2a300796235f169da857fb31670e707420b)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.lib 800eb94202647c8c1245efcf698c76bec5db41a7)
        ELSE()
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.dll 02fd4e51d562fb4bc02db024dbedf6e5806645b8)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.lib aa93ee2add4cad1e53fc3f66ccb773bd3ab0598e)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.dll 02fd4e51d562fb4bc02db024dbedf6e5806645b8)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.lib aa93ee2add4cad1e53fc3f66ccb773bd3ab0598e)
        ENDIF()
    ELSE()
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.dll e2fb73caa0cb20d6722a66638ce2a26233dc875f)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.lib eab202a68ddcb5ab0391ed149a5423d9222ffd42)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.dll e2fb73caa0cb20d6722a66638ce2a26233dc875f)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.lib eab202a68ddcb5ab0391ed149a5423d9222ffd42)
        ELSE()
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.dll db2df210af341e833fb79021ddda4ce1b8b31d0b)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.lib 35d17fda6341331e1fb532f7b0742fd4ea94fe72)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.dll db2df210af341e833fb79021ddda4ce1b8b31d0b)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.lib 35d17fda6341331e1fb532f7b0742fd4ea94fe72)
        ENDIF()
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.0.dylib 288590236cb1a6d8b2389ec7963e16bfc41f0a24)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.1.dylib a9dd0b023f1218f31081b3741d80832dae3941fe)
ELSE()
    IF(${ARCHITECTURE} EQUAL 32)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.0 58938c4bba3e0ce69fe3fdb37a1e22b0f2c990be)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.1 58938c4bba3e0ce69fe3fdb37a1e22b0f2c990be)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.0 95861c1b47b81b0693dbf0fdddc976518828f586)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.1 95861c1b47b81b0693dbf0fdddc976518828f586)
    ENDIF()
ENDIF()

# Retrieve some BioSignalML API settings

RETRIEVE_BioSignalML_API_SETTINGS()

# Add the plugin

SET(BioSignalML_API_EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

ADD_PLUGIN(BioSignalMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/biosignalmlapiplugin.cpp
    HEADERS_MOC
        src/biosignalmlapiplugin.h
    INCLUDE_DIRS
        include
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${BioSignalML_API_EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${BioSignalML_API_EXTERNAL_BINARIES}
    TESTS
        tests
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR}
                           libbiosignalml${CMAKE_SHARED_LIBRARY_SUFFIX})
    WINDOWS_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR}
                           libtypedobject${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR} biosignalml.0
                        typedobject.1)
    OS_X_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR} typedobject.1)
ELSE()
    FOREACH(BioSignalML_API_EXTERNAL_BINARY ${BioSignalML_API_EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR}
                             ${BioSignalML_API_EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
