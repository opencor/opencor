PROJECT(ClangPlugin)

# Add the plugin

SET(Clang_PLUGIN
    Clang

    PARENT_SCOPE
)

ADD_PLUGIN(Clang
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/clangplugin.cpp
    HEADERS_MOC
        src/clangplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
