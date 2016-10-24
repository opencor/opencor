PROJECT(OAuthPlugin)

IF(USE_PREBUILT_OAUTH_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 77c1b6a7a3e6f427c96e04d7e35cde579354d69b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib 9195d5eabedf3f010db91c506b4e870506ea6005)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 521a6d14e491dad18cee081a10c7267c16b7fc92)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib a1ea677120ad2766806312b7f0ed103d91e0ee1a)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.dylib 7da0b2ef37423fda8c7b31a360a2273b4f6d010a)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.so 16aa28d10d725d0336de69532ab6d6facd34dc38)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(OAUTH_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}OAuth${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(OAUTH_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}OAuth${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(OAuth
        INCLUDE_DIRS
            src
    )
ELSE()
    # Add the plugin

    SET(OAUTH_PLUGIN OAuth PARENT_SCOPE)

    ADD_PLUGIN(OAuth
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/o0baseauth.cpp
            src/o0settingsstore.cpp
            src/o1.cpp
            src/o1requestor.cpp
            src/o1timedreply.cpp
            src/o2.cpp
            src/o2reply.cpp
            src/o2replyserver.cpp
            src/o2requestor.cpp
            src/o2simplecrypt.cpp
            src/oauthplugin.cpp
        HEADERS_MOC
            src/o0abstractstore.h
            src/o0baseauth.h
            src/o0settingsstore.h
            src/o1.h
            src/o1requestor.h
            src/o1timedreply.h
            src/o2.h
            src/o2reply.h
            src/o2replyserver.h
            src/o2requestor.h
            src/oauthplugin.h
        INCLUDE_DIRS
            src
        QT_MODULES
            Core
            Network
    )
ENDIF()
