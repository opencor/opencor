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

        Qt4/ListBoxQt.cpp
        Qt4/PlatQt.cpp
        Qt4/qsciabstractapis.cpp
        Qt4/qsciapis.cpp
        Qt4/qscicommand.cpp
        Qt4/qscicommandset.cpp
        Qt4/qscidocument.cpp
        Qt4/qscilexer.cpp
        Qt4/qscilexercpp.cpp
        Qt4/qscilexercss.cpp
        Qt4/qscilexerhtml.cpp
        Qt4/qscilexerjavascript.cpp
        Qt4/qscilexerpython.cpp
        Qt4/qscilexerxml.cpp
        Qt4/qsciscintilla.cpp
        Qt4/qsciscintillabase.cpp
        Qt4/qscistyle.cpp
        Qt4/qscistyledtext.cpp
        Qt4/SciClasses.cpp
        Qt4/ScintillaQt.cpp

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
        Qt4/SciClasses.h

        Qt4/Qsci/qsciabstractapis.h
        Qt4/Qsci/qsciapis.h
        Qt4/Qsci/qscilexer.h
        Qt4/Qsci/qscilexercpp.h
        Qt4/Qsci/qscilexercss.h
        Qt4/Qsci/qscilexerhtml.h
        Qt4/Qsci/qscilexerjavascript.h
        Qt4/Qsci/qscilexerpython.h
        Qt4/Qsci/qscilexerxml.h
        Qt4/Qsci/qsciscintilla.h
        Qt4/Qsci/qsciscintillabase.h

        src/qscintillaplugin.h
    DEFINITIONS
        QSCINTILLA_MAKE_DLL
        QT
        SCI_LEXER
    INCLUDE_DIRS
        include
        lexlib
        Qt4
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
