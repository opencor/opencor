PROJECT(QJsonPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(QJson
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
    HEADERS
        ../../plugininfo.h

        src/json_parser.hh
        src/json_scanner.h
        src/location.hh
        src/parser.h
        src/position.hh
        src/qjson_debug.h
        src/qjson_export.h
        src/qobjecthelper.h
        src/serializer.h
        src/stack.hh
    HEADERS_MOC
        src/parserrunnable.h
        src/qjsonplugin.h
        src/serializerrunnable.h
    DEFINITIONS
        QJSON_MAKEDLL
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
