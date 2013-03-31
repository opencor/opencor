PROJECT(QScintillaPlugin)

# Add the plugin

ADD_PLUGIN(QScintilla
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        lexers/LexHTML.cpp

        lexlib/Accessor.cpp
        lexlib/CharacterSet.cpp
        lexlib/LexerBase.cpp
        lexlib/LexerModule.cpp
        lexlib/LexerSimple.cpp
        lexlib/PropSetSimple.cpp
        lexlib/StyleContext.cpp
        lexlib/WordList.cpp

        Qt4Qt5/ListBoxQt.cpp
        Qt4Qt5/PlatQt.cpp
        Qt4Qt5/qsciabstractapis.cpp
        Qt4Qt5/qsciapis.cpp
        Qt4Qt5/qscicommand.cpp
        Qt4Qt5/qscicommandset.cpp
        Qt4Qt5/qscidocument.cpp
        Qt4Qt5/qscilexer.cpp
        Qt4Qt5/qscilexercpp.cpp
        Qt4Qt5/qscilexercss.cpp
        Qt4Qt5/qscilexerhtml.cpp
        Qt4Qt5/qscilexerjavascript.cpp
        Qt4Qt5/qscilexerpython.cpp
        Qt4Qt5/qscilexerxml.cpp
        Qt4Qt5/qsciscintilla.cpp
        Qt4Qt5/qsciscintillabase.cpp
        Qt4Qt5/qscistyle.cpp
        Qt4Qt5/qscistyledtext.cpp
        Qt4Qt5/SciClasses.cpp
        Qt4Qt5/ScintillaQt.cpp

        src/AutoComplete.cpp
        src/CallTip.cpp
        src/Catalogue.cpp
        src/CellBuffer.cpp
        src/CharClassify.cpp
        src/ContractionState.cpp
        src/Decoration.cpp
        src/Document.cpp
        src/Editor.cpp
        src/Indicator.cpp
        src/KeyMap.cpp
        src/LineMarker.cpp
        src/PerLine.cpp
        src/PositionCache.cpp
        src/qscintillaplugin.cpp
        src/RESearch.cpp
        src/RunStyles.cpp
        src/ScintillaBase.cpp
        src/Selection.cpp
        src/Style.cpp
        src/UniConversion.cpp
        src/ViewStyle.cpp
        src/XPM.cpp
    HEADERS_MOC
        Qt4Qt5/SciClasses.h

        Qt4Qt5/Qsci/qsciabstractapis.h
        Qt4Qt5/Qsci/qsciapis.h
        Qt4Qt5/Qsci/qscilexer.h
        Qt4Qt5/Qsci/qscilexercpp.h
        Qt4Qt5/Qsci/qscilexercss.h
        Qt4Qt5/Qsci/qscilexerhtml.h
        Qt4Qt5/Qsci/qscilexerjavascript.h
        Qt4Qt5/Qsci/qscilexerpython.h
        Qt4Qt5/Qsci/qscilexerxml.h
        Qt4Qt5/Qsci/qsciscintilla.h
        Qt4Qt5/Qsci/qsciscintillabase.h

        src/qscintillaplugin.h
    DEFINITIONS
        QSCINTILLA_MAKE_DLL
        QT
        SCI_LEXER
    INCLUDE_DIRS
        include
        lexlib
        Qt4Qt5
        src
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtWidgets
)
