PROJECT(BioSignalMLAPIPlugin)

MACRO(RETRIEVE_BioSignalML_API_SETTINGS)
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

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll e441a499ae510dd1d1d202d54ca61fd06e5c7972)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 62d44d40d0f5ab044973e713016ce376f0b0cad4)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 94345a2e65536c57a020e3d97fa402abc5ca32f2)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 845408aeb7508baa7698fc77f26f1c2d80550be3)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 0810a7d8bc7cedfd79fa617983be5f4e50aacb75)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 4fa1c9f7df1ac2dc4f325517a1b2f41e5d214c3f)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 65938812d4b6ccec3f008a6afe51dae974903a60)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 650acf146eb221db86d42e8d764da4d3c8bbdcad)
    ENDIF()
ELSEIF(APPLE)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.0.dylib 08c77f2e5ff4a983674b4a014f9394505a8314f7)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.1.dylib 616459ae337435e8dae5c075049d981f4354431d)
ELSE()
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.0 00a6e77a7d968c588db6863eef87888bd8fc63b6)
    RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.1 db203b41a098529958a6bf05b2eac2db11342304)
ENDIF()

# Retrieve some BioSignalML API settings

RETRIEVE_BioSignalML_API_SETTINGS()

# Add the plugin

SET(BioSignalML_API_EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

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
