PROJECT(CellMLSupportPlugin)

# Add the plugin

ADD_PLUGIN(CellMLSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../solverinterface.cpp

        src/cellmlfile.cpp
        src/cellmlfilecellml10exporter.cpp
        src/cellmlfilecellml11exporter.cpp
        src/cellmlfilecellmlexporter.cpp
        src/cellmlfileexporter.cpp
        src/cellmlfileissue.cpp
        src/cellmlfilemanager.cpp
        src/cellmlfilerdftriple.cpp
        src/cellmlfilerdftripleelement.cpp
        src/cellmlfileruntime.cpp
        src/cellmlinterface.cpp
        src/cellmlsupportplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/cellmlfile.h
        src/cellmlfilecellml10exporter.h
        src/cellmlfilemanager.h
        src/cellmlfileruntime.h
        src/cellmlsupportplugin.h
    PLUGINS
        CellMLAPI
        Compiler
        StandardSupport
    TESTS
        tests
)
