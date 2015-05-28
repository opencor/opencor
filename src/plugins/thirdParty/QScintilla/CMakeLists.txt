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
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll d1583516d4083c773873c96eade9d4e2a1eca2e2)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib a8cdf146f265d5caf593a99d3c3c2d65162839b7)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll cb1f4a36ba9ac437233b918aef363446ed336021)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib 6a7eef5f312d26735a7932a71988b5448cb93960)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 769b5f92384c908375500b2da9988e94868dbd2e)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib fa072546cf0c79bf8fe4091fc1ac8458df3dfad7)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 80dd79c0ce119b27d9a7f4b6fe9de40650e5cc5d)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib f068828c72f8fda423e33e492a18ebaf4139373e)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.dylib 22326a65f148d41016d5095841dde098fe58a855)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so 53ca1a8fa7005d00f2d0cf66a84c03b9e2ba4e37)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so 11790d8b110a2d51a1499565f33f2e7cefaaa356)
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
