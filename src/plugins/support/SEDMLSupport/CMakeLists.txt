project(SEDMLSupportPlugin)

# Add the plugin

add_plugin(SEDMLSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/sedmlfile.cpp
        src/sedmlfileissue.cpp
        src/sedmlfilemanager.cpp
        src/sedmlinterface.cpp
        src/sedmlsupport.cpp
        src/sedmlsupportplugin.cpp
    PLUGINS
        CellMLSupport
        libSBML
        libSEDML
    TESTS
        tests
)
