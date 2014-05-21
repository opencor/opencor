PROJECT(QScintillaPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_QSCINTILLA_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll ea5a10948a255a55f9402873d8dbaee5dbe05196)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib c60aed038bfd3dd93aaa4700f0420b82ceecb1d4)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 3392158b603232f0161a0f86e8813ef2bcb773b0)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib f27002bc699f6441d1063b96ec79e7b1f05f0093)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.dylib 9c2b19164aafb885c9acb9652c5d66e52bd323ab)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so bab0c8ae661d69e0be776840c4eee99540e41564)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so cfe026b6c1382c9da2c8100e8d536f26ce7f4bfa)
        ENDIF()
    ENDIF()
ELSE()
    # Add the plugin

    IF(APPLE)
        SET(QT_MODULES_EXTRAS MacExtras)
        SET(QT_LIBRARIES_EXTRAS QtMacExtras)
    ELSE()
        SET(QT_MODULES_EXTRAS)
        SET(QT_LIBRARIES_EXTRAS)
    ENDIF()

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

            Qt4Qt5/InputMethod.cpp
            Qt4Qt5/ListBoxQt.cpp
            Qt4Qt5/MacPasteboardMime.cpp
            Qt4Qt5/PlatQt.cpp
            Qt4Qt5/qsciabstractapis.cpp
            Qt4Qt5/qsciapis.cpp
            Qt4Qt5/qscicommand.cpp
            Qt4Qt5/qscicommandset.cpp
            Qt4Qt5/qscidocument.cpp
            Qt4Qt5/qscilexer.cpp
            Qt4Qt5/qscilexercpp.cpp
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
            src/CaseConvert.cpp
            src/CaseFolder.cpp
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
            Qt4Qt5/Qsci/qscilexerhtml.h
            Qt4Qt5/Qsci/qscilexerjavascript.h
            Qt4Qt5/Qsci/qscilexerpython.h
            Qt4Qt5/Qsci/qscilexerxml.h
            Qt4Qt5/Qsci/qsciscintilla.h
            Qt4Qt5/Qsci/qsciscintillabase.h

            src/qscintillaplugin.h
        INCLUDE_DIRS
            include
            lexlib
            Qt4Qt5
            src
        DEFINITIONS
            QSCINTILLA_MAKE_DLL
            SCINTILLA_QT
            SCI_LEXER
        QT_MODULES
            ${QT_MODULES_EXTRAS}
            Widgets
        QT_LIBRARIES
            ${QT_LIBRARIES_EXTRAS}
            QtCore
            QtGui
            QtWidgets
    )
ENDIF()
