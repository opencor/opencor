project(CellMLTextViewPlugin)

# Add the plugin

add_plugin(CellMLTextView
    SOURCES
        ../../cliinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../CellMLEditingView/src/cellmleditingviewinterface.cpp
        ../EditingView/src/editingviewinterface.cpp

        src/cellmltextviewconverter.cpp
        src/cellmltextviewlexer.cpp
        src/cellmltextviewparser.cpp
        src/cellmltextviewplugin.cpp
        src/cellmltextviewscanner.cpp
        src/cellmltextviewwidget.cpp
    HEADERS_MOC
        src/cellmltextviewconverter.h
        src/cellmltextviewlexer.h
        src/cellmltextviewparser.h
        src/cellmltextviewplugin.h
        src/cellmltextviewscanner.h
        src/cellmltextviewwidget.h
    PLUGINS
        CellMLEditingView
    TESTS
        clitests
        conversiontests
        parsingtests
        scanningtests
)
