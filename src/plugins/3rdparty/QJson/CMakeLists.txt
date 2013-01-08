PROJECT(QJsonPlugin)

# Add the plugin

ADD_PLUGIN(QJson
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/json_parser.cc
        src/json_scanner.cpp
        src/parser.cpp
        src/parserrunnable.cpp
        src/qjsonplugin.cpp
        src/qobjecthelper.cpp
        src/serializer.cpp
        src/serializerrunnable.cpp
    HEADERS_MOC
        src/parserrunnable.h
        src/qjsonplugin.h
        src/serializerrunnable.h
    DEFINITIONS
        QJSON_MAKEDLL
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_DEPENDENCIES
        QtCore
)
