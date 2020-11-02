project(CorePlugin)

# Add the plugin

if(APPLE)
    set(TABBARWIDGETMACSTYLE_CPP src/tabbarwidgetmacstyle.cpp)
endif()

add_plugin(Core
    SOURCES
        ../../cliinterface.cpp
        ../../datastoreinterface.cpp
        ../../coreinterface.cpp
        ../../filehandlinginterface.cpp
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp
        ../../preferencesinterface.cpp
        ../../pythoninterface.cpp
        ../../solverinterface.cpp
        ../../viewinterface.cpp
        ../../windowinterface.cpp

        ${TABBARWIDGETMACSTYLE_CPP}

        src/borderedwidget.cpp
        src/busywidget.cpp
        src/centralwidget.cpp
        src/collapsiblewidget.cpp
        src/commonwidget.cpp
        src/corecliutils.cpp
        src/coreguiutils.cpp
        src/coreplugin.cpp
        src/file.cpp
        src/filemanager.cpp
        src/interfaces.cpp
        src/mathmlconverter.cpp
        src/organisationwidget.cpp
        src/progressbarwidget.cpp
        src/propertyeditorwidget.cpp
        src/remotefiledialog.cpp
        src/splitterwidget.cpp
        src/tabbarwidget.cpp
        src/treeviewwidget.cpp
        src/usermessagewidget.cpp
        src/viewwidget.cpp
        src/widget.cpp
        src/windowwidget.cpp
        src/xsltransformer.cpp
    QT_MODULES
        Network
        Widgets
        Xml
        XmlPatterns
    SYSTEM_BINARIES
        ${PYTHON_LIBRARY}
    TESTS
        clitests
        generaltests
        mathmltests
    DEPENDS_ON
        ${PYTHON_DEPENDENCIES}
)
