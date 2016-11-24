PROJECT(BioSignalMLAPIPlugin)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

SET(BIOSIGNALML_MAJOR_VERSION "0")
SET(TYPEDOBJECT_MAJOR_VERSION "1")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 94bc0c933d41d1e317e3568576dec96077239271)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 49b708bb230a8be7ae0a2eaa17eb0bd44c6f6158)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 037b31ac63d2e4a809b29bbb35056de37b8fb3fd)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 37db06977e3f26317afcf4400b0fefc99994a432)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 779f4064b06ad4d8ec2998ed8553dec16f815c93)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 8ab6470168ad40a1717e64d46c0e8eaab53ebb85)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 2fef49bbfc259656e698bc0b3749a928c254999c)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 34f28e57932941cd06f5acef49a46cb38b5185b1)
    ENDIF()
ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.${BIOSIGNALML_MAJOR_VERSION}.dylib c12d1455b71dd2c19107edf02bb59c6ed78d917a)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.${TYPEDOBJECT_MAJOR_VERSION}.dylib c45259f4e0d64c44c343201f497a67c5489f9e54)
ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libbiosignalml.so.${BIOSIGNALML_MAJOR_VERSION} 0c2f927841fa7e474745e38db1d803e570bf9e5e)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} libtypedobject.so.${TYPEDOBJECT_MAJOR_VERSION} d0e619e47c28e029fa51db15e534fd201154a1bd)
ENDIF()

# Retrieve the BioSignalML API external binaries

SET(EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

IF(WIN32)
    SET(EXTERNAL_BINARIES
        biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX}
        typedobject${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSEIF(APPLE)
    SET(EXTERNAL_BINARIES
        libbiosignalml.${BIOSIGNALML_MAJOR_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
        libtypedobject.${TYPEDOBJECT_MAJOR_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    SET(EXTERNAL_BINARIES
        libbiosignalml${CMAKE_SHARED_LIBRARY_SUFFIX}.${BIOSIGNALML_MAJOR_VERSION}
        libtypedobject${CMAKE_SHARED_LIBRARY_SUFFIX}.${TYPEDOBJECT_MAJOR_VERSION}
    )
ENDIF()

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
