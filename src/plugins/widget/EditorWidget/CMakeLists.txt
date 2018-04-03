project(EditorWidgetPlugin)

# Add the plugin
# Note: on Windows, we must reference a couple of QScintilla header files so
#       that we can use the new signal/slot mechanism (see EditorWidget's
#       constructor). Since we have to do it for Windows, we might as well do it
#       for Linux and macOS too...

if(WIN32)
    if(RELEASE_MODE)
        set(MODE_DIR release)
    else()
        set(MODE_DIR debug)
    endif()
endif()

set(QSCISCINTILLA_HEADERS_MOC
    ../../thirdParty/QScintilla/ext/${MODE_DIR}/include/Qsci/qsciscintilla.h
    ../../thirdParty/QScintilla/ext/${MODE_DIR}/include/Qsci/qsciscintillabase.h
)

add_plugin(EditorWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorlistwidget.cpp
        src/editorwidget.cpp
        src/editorwidgetfindreplacewidget.cpp
        src/editorwidgetplugin.cpp
    HEADERS_MOC
        ${QSCISCINTILLA_HEADERS_MOC}
        src/editorlistwidget.h
        src/editorwidget.h
        src/editorwidgetfindreplacewidget.h
        src/editorwidgetplugin.h
    UIS
        src/editorwidgetfindreplacewidget.ui
    PLUGINS
        QScintillaSupport
)
