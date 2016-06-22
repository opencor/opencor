PROJECT(OAuthPlugin)

# Add the plugin

SET(OAUTH_PLUGIN OAuth PARENT_SCOPE)

ADD_PLUGIN(OAuth
    THIRD_PARTY
    DEFINITIONS
        QT_NO_DEBUG_OUTPUT
    SOURCES
        ../../plugininfo.cpp
        o2/src/o0baseauth.cpp
        o2/src/o0globals.h
        o2/src/o0settingsstore.cpp
        o2/src/o1.cpp
        o2/src/o1requestor.cpp
        o2/src/o1timedreply.cpp
        o2/src/o2.cpp
        o2/src/o2reply.cpp
        o2/src/o2replyserver.cpp
        o2/src/o2requestor.cpp
        o2/src/o2simplecrypt.cpp
        src/oauthplugin.cpp
    HEADERS_MOC
        o2/src/o0abstractstore.h
        o2/src/o0baseauth.h
        o2/src/o0settingsstore.h
        o2/src/o1.h
        o2/src/o1requestor.h
        o2/src/o1timedreply.h
        o2/src/o2.h
        o2/src/o2reply.h
        o2/src/o2replyserver.h
        o2/src/o2requestor.h
        src/oauthplugin.h
    INCLUDE_DIRS
        o2/src
    QT_MODULES
        Core
        Network
)
