PROJECT(BioSignalMLAPIPlugin)

MACRO(RETRIEVE_BioSignalML_API_SETTINGS)
    # Retrieve some BioSignalML API settings

    IF(WIN32)
        SET(BioSignalML_API_EXTERNAL_BINARIES
            biosignalml${CMAKE_IMPORT_LIBRARY_SUFFIX}
            typedobject${CMAKE_IMPORT_LIBRARY_SUFFIX}
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
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 8)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 8)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 8)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 8)
        ELSE()
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 0)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib a)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 0)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib a)
        ENDIF()
    ELSE()
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 8bd693d6a425464dfe55f5603fba76f033080171)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib b8b848fdb91de2946fdedb09d02d7c25296c1742)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 8cd27aecac5474d9e71a6b03b9aedd643a6f16ae)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 8b401b600f9a7cdf74219716dc5f22b6783d725e)
        ELSE()
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll d)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 3)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll d)
            RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 3)
        ENDIF()
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.0.dylib bced20f9b8a00eaf14a2b7e94d2de44882feb465)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.1.dylib a1afd0ec6e063ae0fe9502333aecfca4826e81ab)
ELSE()
    IF(${ARCHITECTURE} EQUAL 32)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.0 5)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.1 5)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.0 9)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.1 9)
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
#    TESTS
#        tests
)

# Deploy the plugin's external binaries
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external binary may have dependencies (something that is important on OS
#       X)...

IF(WIN32)
    WINDOWS_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR}
                           biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX})
    WINDOWS_DEPLOY_LIBRARY(${BioSignalML_API_EXTERNAL_BINARIES_DIR}
                           typedobject${CMAKE_SHARED_LIBRARY_SUFFIX})
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
