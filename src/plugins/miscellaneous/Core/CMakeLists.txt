PROJECT(CorePlugin)

# Add the plugin

IF(WIN32)
    SET(PLATFORM_SPECIFIC_CPP qtlockedfile_win.cpp)
ELSE()
    SET(PLATFORM_SPECIFIC_CPP qtlockedfile_unix.cpp)
ENDIF()

ADD_PLUGIN(Core
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp
        ../../viewinterface.cpp

        src/borderedwidget.cpp
        src/busysupportwidget.cpp
        src/busywidget.cpp
        src/centralwidget.cpp
        src/collapsiblewidget.cpp
        src/commonwidget.cpp
        src/corecliutils.cpp
        src/coreguiutils.cpp
        src/coreplugin.cpp
        src/file.cpp
        src/filemanager.cpp
        src/mathmlconverter.cpp
        src/organisationwidget.cpp
        src/progressbarwidget.cpp
        src/propertyeditorwidget.cpp
        src/tabbarwidget.cpp
        src/toolbarwidget.cpp
        src/treeviewwidget.cpp
        src/usermessagewidget.cpp
        src/viewwidget.cpp
        src/widget.cpp
        src/windowwidget.cpp
        src/xsltransformer.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/borderedwidget.h
        src/busywidget.h
        src/centralwidget.h
        src/collapsiblewidget.h
        src/corecliutils.h
        src/coreplugin.h
        src/filemanager.h
        src/mathmlconverter.h
        src/organisationwidget.h
        src/progressbarwidget.h
        src/propertyeditorwidget.h
        src/tabbarwidget.h
        src/toolbarwidget.h
        src/treeviewwidget.h
        src/usermessagewidget.h
        src/viewwidget.h
        src/widget.h
        src/windowwidget.h
        src/xsltransformer.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Network
        Widgets
        Xml
        XmlPatterns
    TESTS
        generaltests
        mathmltests
)
