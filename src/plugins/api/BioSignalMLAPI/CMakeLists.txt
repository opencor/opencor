PROJECT(BioSignalMLAPIPlugin)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

SET(BIOSIGNALML_VERSION "0.8")
SET(TYPEDOBJECT_VERSION "1.1")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.${BIOSIGNALML_VERSION}.dll 8b921ea16fbe7301f30906ab54ef3091d0ac9007)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.${BIOSIGNALML_VERSION}.lib 4cba82bd62c8424534f9bfa9d8fbeec045ba333b)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.${TYPEDOBJECT_VERSION}.dll b522b893a2ec1af5c78211f004912381779825cd)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.${TYPEDOBJECT_VERSION}.lib d13be1757b6f55a485a99e5d778afbe6add1748e)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.${BIOSIGNALML_VERSION}.dll 5a8ff057b3e5ad3f1c582ecd2ff39d590bb9b971)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.${BIOSIGNALML_VERSION}.lib 33cf0f26904957958c24f414299045d754fd5a8d)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.${TYPEDOBJECT_VERSION}.dll 0ae04c24dde8170688dba8dc4c87bf62c9c09d15)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.${TYPEDOBJECT_VERSION}.lib c20fc5c0c63ceb2acabdbec292a9bfbee4536484)
    ENDIF()
ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.${BIOSIGNALML_VERSION}.dylib b5966cd7f43e16d731e873fb407a1d843314ff2d)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.${TYPEDOBJECT_VERSION}.dylib 5520123168c199b076cce2bb0212081164c6ecd0)
ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.${BIOSIGNALML_VERSION} ef827c75a63f24b069fd83ecaba56debfa9764bd)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.${TYPEDOBJECT_VERSION} bc5395e1bbb650c50e79258bae6b9aaef4ef4a2a)
ENDIF()

# Retrieve the BioSignalML API external binaries

SET(EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

IF(WIN32)
    SET(EXTERNAL_BINARIES
        biosignalml.${BIOSIGNALML_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX}
        typedobject.${TYPEDOBJECT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX}
    )
ELSEIF(APPLE)
    SET(EXTERNAL_BINARIES
        libbiosignalml.${BIOSIGNALML_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
        libtypedobject.${TYPEDOBJECT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    SET(EXTERNAL_BINARIES
        libbiosignalml${CMAKE_SHARED_LIBRARY_SUFFIX}.${BIOSIGNALML_VERSION}
        libtypedobject${CMAKE_SHARED_LIBRARY_SUFFIX}.${TYPEDOBJECT_VERSION}
    )
ENDIF()

SET(BIOSIGNALML_API_EXTERNAL_BINARIES ${EXTERNAL_BINARIES} PARENT_SCOPE)

# Add the plugin

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
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR}
                           biosignalml.${BIOSIGNALML_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    WINDOWS_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR}
                           typedobject.${TYPEDOBJECT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    OS_X_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} biosignalml.${BIOSIGNALML_VERSION}
                                                                 typedobject.${TYPEDOBJECT_VERSION})
    OS_X_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} typedobject.${TYPEDOBJECT_VERSION})
ELSE()
    FOREACH(EXTERNAL_BINARY ${EXTERNAL_BINARIES})
        LINUX_DEPLOY_LIBRARY(${EXTERNAL_BINARIES_DIR} ${EXTERNAL_BINARY})
    ENDFOREACH()
ENDIF()
