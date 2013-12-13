PROJECT(RawCellMLViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
ENDIF()

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/rawcellmlviewplugin.cpp
        src/rawcellmlviewwidget.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
        src/rawcellmlviewwidget.h
    UIS
        src/rawcellmlviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
        QScintilla
        QScintillaSupport
        Qwt
        Viewer
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        ${QT_LIBRARIES_EXTRAS}
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
