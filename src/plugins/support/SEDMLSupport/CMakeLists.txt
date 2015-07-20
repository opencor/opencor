PROJECT(SEDMLSupportPlugin)

# Retrieve some SED-ML API settings

RETRIEVE_SEDML_API_SETTINGS()

# Add the plugin

ADD_PLUGIN(SEDMLSupport
    SOURCES
        ../../plugininfo.cpp

        src/sedmlfile.cpp
        src/sedmlfilemanager.cpp
        src/sedmlsupportplugin.cpp
    HEADERS_MOC
        src/sedmlfile.h
        src/sedmlfilemanager.h
        src/sedmlsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        SEDMLAPI
        StandardSupport
    EXTERNAL_BINARIES
        ${SEDML_API_EXTERNAL_BINARIES}
)
