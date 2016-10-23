PROJECT(OAuthPlugin)

IF(USE_PREBUILT_OAUTH_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll 51edd00b6a11dd22184ead978274eef998865983)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib d4e58a1891d212412773aae389132cec9fbb4737)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.dll e62a3bfae2d4fdc3938f754a42d9779559145959)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} OAuth.lib f4654e87152261d67547ea4bcf11401ca0f3c513)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.dylib 8457251446112676ea20bf47dbe48cc41f792cfc)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libOAuth.so e3f2c19efb2ef05f9d92a2a2b602c3efd36188c6)
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
            src/o2facebook.cpp
            src/o2gft.cpp
            src/o2hubic.cpp
            src/o2reply.cpp
            src/o2replyserver.cpp
            src/o2requestor.cpp
            src/o2simplecrypt.cpp
            src/o2skydrive.cpp
            src/o2surveymonkey.cpp
            src/oauthplugin.cpp
            src/oxtwitter.cpp
        HEADERS_MOC
            src/o0abstractstore.h
            src/o0baseauth.h
            src/o0settingsstore.h
            src/o1.h
            src/o1dropbox.h
            src/o1flickr.h
            src/o1freshbooks.h
            src/o1requestor.h
            src/o1timedreply.h
            src/o1twitter.h
            src/o2.h
            src/o2facebook.h
            src/o2gft.h
            src/o2hubic.h
            src/o2reply.h
            src/o2replyserver.h
            src/o2requestor.h
            src/o2skydrive.h
            src/o2surveymonkey.h
            src/oauthplugin.h
            src/oxtwitter.h
        INCLUDE_DIRS
            src
        QT_MODULES
            Core
            Network
    )
ENDIF()
