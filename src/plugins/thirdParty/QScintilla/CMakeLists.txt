PROJECT(QScintillaPlugin)

# Use our pre-built version unless instructed otherwise

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

IF(USE_PREBUILT_QSCINTILLA_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 006951e02083ef7ca4a41551e07be9e605845ef2)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 4c277d0e4859ff6ca7d1192bd4b9ff96f4eb3a2d)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 98ba8f744835e9e1eedf1c03e10df903251d5b0f)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 22d92aa13b2d34ebed95b5138134c62069270924)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 1f1fc1312fd89a0aa9bc4ce9e5da766d4cbea2e8)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 3a944ca7f114f67f584cbac45381a7f0ce587fbd)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll c9fc3e80b85dbfac0a92b6ba3a97d150dbc61de5)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 54bd8d479e1fd87f8520726748757fd6e014be4d)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.dylib 5cd2b91ca8d745b76e3d81e55aefc33507facec1)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so ff79926373fc4ad0b5b3e10148502d7ff058ee45)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so bf26bafead4a3ffd20d8fd0d712e9ebc53dd50d4)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(QSCINTILLA_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}QScintilla${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(QSCINTILLA_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}QScintilla${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(QScintilla
        INCLUDE_DIRS
            include
            lexlib
            Qt4Qt5
            src
        QT_LIBRARIES
            QtCore
            QtGui
            ${QT_MAC_EXTRAS}
            QtWidgets
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
        QT_LIBRARIES
            QtCore
            QtGui
            ${QT_MAC_EXTRAS}
            QtWidgets
    )
ENDIF()
