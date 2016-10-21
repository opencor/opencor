PROJECT(OAuthPlugin)

# Add the plugin

SET(OAUTH_PLUGIN OAuth PARENT_SCOPE)

ADD_PLUGIN(OAuth
    THIRD_PARTY
    DEFINITIONS
        QT_NO_DEBUG_OUTPUT
        O2_DLL_EXPORT
    SOURCES
        ../../plugininfo.cpp

        src/o0baseauth.cpp
        src/o0globals.h
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
