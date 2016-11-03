PROJECT(OAuthPlugin)

IF(USE_PREBUILT_OAUTH_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 0a0bfd2220cd5627eabe6b043ce66c1e9a58348a)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib a1eae11a50d234d8e99efc702dc8f2c0784c1951)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 545013de00aa6ced162aebce1cda42b8c22eda75)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib 223fd1d5e8a7851053f924ca3260c640dcece493)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.dylib a1c705b0173aa33106b9e45658722942ff26bf8c)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.so 96d696aeb49ad1cb09d7f94eb81e6712f8867e4a)
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
