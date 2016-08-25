PROJECT(PythonQtPlugin)

add_subdirectory(PythonQt)

# Add the plugin

ADD_PLUGIN(PythonQt
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtplugin.cpp
    HEADERS_MOC
        src/pythonqtplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Python
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${ZLIB_PLUGIN_BINARY}
)
