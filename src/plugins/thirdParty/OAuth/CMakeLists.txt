PROJECT(OAuthPlugin)

IF(USE_PREBUILT_OAUTH_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 02228e8134ff6f2e3510423da7735830a47335e8)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib be9cfc8269f984fa733addaf1336b9f7eaa10326)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll c147f65fc63f04f8fb62025eb6e44067e277f4df)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib bd06c203bd24b5679fb8a112ae7465ce8416c8e3)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.dylib 7b74ae141fa224b22fde8bc8000dfa673f7319db)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.so 392ce5692fdc949f57f6ea31a74decf95224228f)
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
        DEFINITIONS
            O2_DLL_EXPORT
            QT_NO_DEBUG_OUTPUT
            QT_NO_WARNING_OUTPUT
        QT_MODULES
            Core
            Network
    )
ENDIF()
