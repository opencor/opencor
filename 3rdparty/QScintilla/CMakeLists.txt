PROJECT(QScintilla)

# Files that make up QScintilla

SET(SOURCES
    lexers/LexCPP.cpp
    lexers/LexCSS.cpp
    lexers/LexHTML.cpp
    lexers/LexPython.cpp

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
    src/RESearch.cpp
    src/RunStyles.cpp
    src/ScintillaBase.cpp
    src/Selection.cpp
    src/Style.cpp
    src/UniConversion.cpp
    src/ViewStyle.cpp
    src/XPM.cpp
)

SET(HEADERS
    include/ILexer.h
    include/Platform.h
    include/SciLexer.h
    include/Scintilla.h

    lexlib/Accessor.h
    lexlib/CharacterSet.h
    lexlib/LexAccessor.h
    lexlib/LexerBase.h
    lexlib/LexerModule.h
    lexlib/LexerSimple.h
    lexlib/OptionSet.h
    lexlib/PropSetSimple.h
    lexlib/SparseState.h
    lexlib/StyleContext.h
    lexlib/WordList.h

    Qt4/ListBoxQt.h
    Qt4/ScintillaQt.h

    Qt4/Qsci/qscicommand.h
    Qt4/Qsci/qscicommandset.h
    Qt4/Qsci/qscidocument.h
    Qt4/Qsci/qsciglobal.h
    Qt4/Qsci/qscistyle.h
    Qt4/Qsci/qscistyledtext.h

    src/Autocomplete.h
    src/CallTip.h
    src/Catalogue.h
    src/CellBuffer.h
    src/CharClassify.h
    src/ContractionState.h
    src/Decoration.h
    src/Document.h
    src/Editor.h
    src/FontQuality.h
    src/Indicator.h
    src/KeyMap.h
    src/LineMarker.h
    src/Partitioning.h
    src/PerLine.h
    src/PositionCache.h
    src/RESearch.h
    src/RunStyles.h
    src/ScintillaBase.h
    src/Selection.h
    src/SplitVector.h
    src/Style.h
    src/SVector.h
    src/UniConversion.h
    src/ViewStyle.h
    src/XPM.h
)

SET(HEADERS_MOC
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
)

# Various include directories

SET(LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS include lexlib Qt4 Qt4/Qsci src)
SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS ${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS})

# Qt specific

INCLUDE(${QT_USE_FILE})

# Rules to build QScintilla

ADD_DEFINITIONS(-DQT -DSCI_LEXER)

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test QScintilla
#       without first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_MOC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
)

# Compiler and linker options

IF(NOT APPLE AND NOT ${DEBUG_MODE})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-s")
ENDIF()

# Rules to package QScintilla

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()
