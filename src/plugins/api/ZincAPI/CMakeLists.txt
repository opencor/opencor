PROJECT(ZincAPIPlugin)

# Retrieve the plugin's binary file(s)

#STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
#       RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

#IF(WIN32)
#    IF(RELEASE_MODE)
#        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zinc.dll sha1value)
#        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zinc.lib sha1value)
#    ELSE()
#        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zinc.dll sha1value)
#        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zinc.lib sha1value)
#    ENDIF()
#ELSEIF(APPLE)
#    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzinc.dylib sha1value)
#ELSE()
#    RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzinc.so sha1value)
#ENDIF()

# Retrieve the Zinc API external binaries

#SET(EXTERNAL_BINARIES_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR})

#IF(WIN32)
#    SET(EXTERNAL_BINARIES
#        zinc${CMAKE_SHARED_LIBRARY_SUFFIX}
#    )
#ELSEIF(APPLE)
#    SET(EXTERNAL_BINARIES
#        libzinc${CMAKE_SHARED_LIBRARY_SUFFIX}
#    )
#ELSE()
#    SET(EXTERNAL_BINARIES
#        libzinc${CMAKE_SHARED_LIBRARY_SUFFIX}
#    )
#ENDIF()

# Add the plugin

ADD_PLUGIN(ZincAPI
    SOURCES
        ../../plugininfo.cpp

        src/zincapiplugin.cpp
    HEADERS_MOC
        src/zincapiplugin.h
    INCLUDE_DIRS
#        include
        src
    QT_MODULES
        Core
#    EXTERNAL_BINARIES_DIR
#        ${EXTERNAL_BINARIES_DIR}
#    EXTERNAL_BINARIES
#        ${EXTERNAL_BINARIES}
#    TESTS
#        tests
)
