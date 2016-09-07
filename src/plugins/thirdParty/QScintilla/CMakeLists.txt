PROJECT(QScintillaPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_QSCINTILLA_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 7c877ae12b1973e562008b67324aa96fe364c02d)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 7bde5ec42bb78d6813a06e0c1090e4b3414e689f)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll a62d1dc388de84f83c2d44b6092382d67af2a25f)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 64f99bcb3ba5617e43a9524f2158834b854212bd)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.dylib 90052cd1f4b8fe86ff81ec049fd1e27af12e0232)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so 1b765e7980a9a176437b889a4b86b6c93f0cf58d)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(QSCINTILLA_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}QScintilla${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(QSCINTILLA_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}QScintilla${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(QScintilla
        INCLUDE_DIRS
            include
            lexlib
            Qt4Qt5
            src
    )
ELSE()
    # Add the plugin

    IF(APPLE)
        SET(MAC_EXTRAS MacExtras)
    ELSE()
        SET(MAC_EXTRAS)
    ENDIF()

    SET(QSCINTILLA_PLUGIN QScintilla PARENT_SCOPE)

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
            Qt4Qt5/qscilexercustom.cpp
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
            src/EditModel.cpp
            src/Editor.cpp
            src/EditView.cpp
            src/Indicator.cpp
            src/KeyMap.cpp
            src/LineMarker.cpp
            src/MarginView.cpp
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

            Qt4Qt5/SciClasses.h
            Qt4Qt5/ScintillaQt.h

            Qt4Qt5/Qsci/qsciabstractapis.h
            Qt4Qt5/Qsci/qsciapis.h
            Qt4Qt5/Qsci/qscilexer.h
            Qt4Qt5/Qsci/qscilexercpp.h
            Qt4Qt5/Qsci/qscilexercustom.h
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
            ${MAC_EXTRAS}
            Widgets
    )
ENDIF()
