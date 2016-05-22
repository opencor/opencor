PROJECT(BioSignalMLAPIPlugin)

# Retrieve the plugin's binary file(s)

STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

SET(BIOSIGNALML_MAJOR_VERSION "0")
SET(TYPEDOBJECT_MAJOR_VERSION "1")

IF(WIN32)
    IF(RELEASE_MODE)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll fc30a9d29c766af3ff93bf371f7121ddb88cef99)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 2016c0280d94f67f3536b23d07daced87ce837b3)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll 071b92ddb07965711fd2101cf75aa565f4cb0a46)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib c8223fcb7ef826069b2b23531d16963f04c96d22)
    ELSE()
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.dll 2dcdd802bbaccbf7b43f797ba6792a854cb2e406)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} biosignalml.lib 3491868f701feff6d7593b145eef76c2dcd85d26)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.dll eea8111cd4bf722b1e855f237f4b1b68ecb2f1c3)
        RETRIEVE_BINARY_FILE_FROM("http://biosignalml.org/binaries" ${RELATIVE_PROJECT_SOURCE_DIR} typedobject.lib 836d2851910ee789b6e49b54b838968821a7f65a)
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
