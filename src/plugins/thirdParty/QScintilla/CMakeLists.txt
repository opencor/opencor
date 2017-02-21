PROJECT(QScintillaPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_QSCINTILLA_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll a9a953cdf7a81cd1e03c6f70ec6eb704c1527350)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib b891a2a4db44d03054ea10de5ca03804f886c0ad)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.dll 8673d4b2f92d71a8eb94a698f2dc63c07566393b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} QScintilla.lib cd1a2ddddaa48eb83b3bd155a2b48b99a85bf8be)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.dylib 6be482e5e2762f9b8c5b1a3328e19a52a33d1c5c)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQScintilla.so f429b41cdb49be8d56e03a6e72105a89ceaa837c)
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
    # Ignore some warnings on Windows

    IF(WIN32)
        SET(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} /wd4996")
    ENDIF()

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

            lexers/LexA68k.cpp
            lexers/LexAbaqus.cpp
            lexers/LexAda.cpp
            lexers/LexAPDL.cpp
            lexers/LexAsm.cpp
            lexers/LexAsn1.cpp
            lexers/LexASY.cpp
            lexers/LexAU3.cpp
            lexers/LexAVE.cpp
            lexers/LexAVS.cpp
            lexers/LexBaan.cpp
            lexers/LexBash.cpp
            lexers/LexBasic.cpp
            lexers/LexBatch.cpp
            lexers/LexBibTeX.cpp
            lexers/LexBullant.cpp
            lexers/LexCaml.cpp
            lexers/LexCLW.cpp
            lexers/LexCmake.cpp
            lexers/LexCOBOL.cpp
            lexers/LexCoffeeScript.cpp
            lexers/LexConf.cpp
            lexers/LexCPP.cpp
            lexers/LexCrontab.cpp
            lexers/LexCsound.cpp
            lexers/LexCSS.cpp
            lexers/LexD.cpp
            lexers/LexDiff.cpp
            lexers/LexDMAP.cpp
            lexers/LexDMIS.cpp
            lexers/LexECL.cpp
            lexers/LexEDIFACT.cpp
            lexers/LexEiffel.cpp
            lexers/LexErlang.cpp
            lexers/LexErrorList.cpp
            lexers/LexEScript.cpp
            lexers/LexFlagship.cpp
            lexers/LexForth.cpp
            lexers/LexFortran.cpp
            lexers/LexGAP.cpp
            lexers/LexGui4Cli.cpp
            lexers/LexHaskell.cpp
            lexers/LexHex.cpp
            lexers/LexHTML.cpp
            lexers/LexInno.cpp
            lexers/LexJSON.cpp
            lexers/LexKix.cpp
            lexers/LexKVIrc.cpp
            lexers/LexLaTeX.cpp
            lexers/LexLisp.cpp
            lexers/LexLout.cpp
            lexers/LexLua.cpp
            lexers/LexMagik.cpp
            lexers/LexMake.cpp
            lexers/LexMarkdown.cpp
            lexers/LexMatlab.cpp
            lexers/LexMetapost.cpp
            lexers/LexMMIXAL.cpp
            lexers/LexModula.cpp
            lexers/LexMPT.cpp
            lexers/LexMSSQL.cpp
            lexers/LexMySQL.cpp
            lexers/LexNimrod.cpp
            lexers/LexNsis.cpp
            lexers/LexNull.cpp
            lexers/LexOpal.cpp
            lexers/LexOScript.cpp
            lexers/LexPascal.cpp
            lexers/LexPB.cpp
            lexers/LexPerl.cpp
            lexers/LexPLM.cpp
            lexers/LexPO.cpp
            lexers/LexPOV.cpp
            lexers/LexPowerPro.cpp
            lexers/LexPowerShell.cpp
            lexers/LexProgress.cpp
            lexers/LexProps.cpp
            lexers/LexPS.cpp
            lexers/LexPython.cpp
            lexers/LexR.cpp
            lexers/LexRebol.cpp
            lexers/LexRegistry.cpp
            lexers/LexRuby.cpp
            lexers/LexRust.cpp
            lexers/LexScriptol.cpp
            lexers/LexSmalltalk.cpp
            lexers/LexSML.cpp
            lexers/LexSorcus.cpp
            lexers/LexSpecman.cpp
            lexers/LexSpice.cpp
            lexers/LexSQL.cpp
            lexers/LexSTTXT.cpp
            lexers/LexTACL.cpp
            lexers/LexTADS3.cpp
            lexers/LexTAL.cpp
            lexers/LexTCL.cpp
            lexers/LexTCMD.cpp
            lexers/LexTeX.cpp
            lexers/LexTxt2tags.cpp
            lexers/LexVB.cpp
            lexers/LexVerilog.cpp
            lexers/LexVHDL.cpp
            lexers/LexVisualProlog.cpp
            lexers/LexYAML.cpp

            lexlib/Accessor.cpp
            lexlib/CharacterCategory.cpp
            lexlib/CharacterSet.cpp
            lexlib/LexerBase.cpp
            lexlib/LexerModule.cpp
            lexlib/LexerNoExceptions.cpp
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
            Qt4Qt5/qscilexeravs.cpp
            Qt4Qt5/qscilexerbash.cpp
            Qt4Qt5/qscilexerbatch.cpp
            Qt4Qt5/qscilexercmake.cpp
            Qt4Qt5/qscilexercoffeescript.cpp
            Qt4Qt5/qscilexercpp.cpp
            Qt4Qt5/qscilexercsharp.cpp
            Qt4Qt5/qscilexercss.cpp
            Qt4Qt5/qscilexercustom.cpp
            Qt4Qt5/qscilexerd.cpp
            Qt4Qt5/qscilexerdiff.cpp
            Qt4Qt5/qscilexerfortran.cpp
            Qt4Qt5/qscilexerfortran77.cpp
            Qt4Qt5/qscilexerhtml.cpp
            Qt4Qt5/qscilexeridl.cpp
            Qt4Qt5/qscilexerjava.cpp
            Qt4Qt5/qscilexerjavascript.cpp
            Qt4Qt5/qscilexerjson.cpp
            Qt4Qt5/qscilexerlua.cpp
            Qt4Qt5/qscilexermakefile.cpp
            Qt4Qt5/qscilexermarkdown.cpp
            Qt4Qt5/qscilexermatlab.cpp
            Qt4Qt5/qscilexeroctave.cpp
            Qt4Qt5/qscilexerpascal.cpp
            Qt4Qt5/qscilexerperl.cpp
            Qt4Qt5/qscilexerpo.cpp
            Qt4Qt5/qscilexerpostscript.cpp
            Qt4Qt5/qscilexerpov.cpp
            Qt4Qt5/qscilexerproperties.cpp
            Qt4Qt5/qscilexerpython.cpp
            Qt4Qt5/qscilexerruby.cpp
            Qt4Qt5/qscilexerspice.cpp
            Qt4Qt5/qscilexersql.cpp
            Qt4Qt5/qscilexertcl.cpp
            Qt4Qt5/qscilexertex.cpp
            Qt4Qt5/qscilexerverilog.cpp
            Qt4Qt5/qscilexervhdl.cpp
            Qt4Qt5/qscilexerxml.cpp
            Qt4Qt5/qscilexeryaml.cpp
            Qt4Qt5/qscimacro.cpp
            Qt4Qt5/qsciprinter.cpp
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
            src/ExternalLexer.cpp
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
            Qt4Qt5/ScintillaQt.h

            Qt4Qt5/Qsci/qsciabstractapis.h
            Qt4Qt5/Qsci/qsciapis.h
            Qt4Qt5/Qsci/qscilexer.h
            Qt4Qt5/Qsci/qscilexeravs.h
            Qt4Qt5/Qsci/qscilexerbash.h
            Qt4Qt5/Qsci/qscilexerbatch.h
            Qt4Qt5/Qsci/qscilexercmake.h
            Qt4Qt5/Qsci/qscilexercoffeescript.h
            Qt4Qt5/Qsci/qscilexercpp.h
            Qt4Qt5/Qsci/qscilexercsharp.h
            Qt4Qt5/Qsci/qscilexercss.h
            Qt4Qt5/Qsci/qscilexercustom.h
            Qt4Qt5/Qsci/qscilexerd.h
            Qt4Qt5/Qsci/qscilexerdiff.h
            Qt4Qt5/Qsci/qscilexerfortran.h
            Qt4Qt5/Qsci/qscilexerfortran77.h
            Qt4Qt5/Qsci/qscilexerhtml.h
            Qt4Qt5/Qsci/qscilexeridl.h
            Qt4Qt5/Qsci/qscilexerjava.h
            Qt4Qt5/Qsci/qscilexerjavascript.h
            Qt4Qt5/Qsci/qscilexerjson.h
            Qt4Qt5/Qsci/qscilexerlua.h
            Qt4Qt5/Qsci/qscilexermakefile.h
            Qt4Qt5/Qsci/qscilexermarkdown.h
            Qt4Qt5/Qsci/qscilexermatlab.h
            Qt4Qt5/Qsci/qscilexeroctave.h
            Qt4Qt5/Qsci/qscilexerpascal.h
            Qt4Qt5/Qsci/qscilexerperl.h
            Qt4Qt5/Qsci/qscilexerpo.h
            Qt4Qt5/Qsci/qscilexerpostscript.h
            Qt4Qt5/Qsci/qscilexerpov.h
            Qt4Qt5/Qsci/qscilexerproperties.h
            Qt4Qt5/Qsci/qscilexerpython.h
            Qt4Qt5/Qsci/qscilexerruby.h
            Qt4Qt5/Qsci/qscilexerspice.h
            Qt4Qt5/Qsci/qscilexersql.h
            Qt4Qt5/Qsci/qscilexertcl.h
            Qt4Qt5/Qsci/qscilexertex.h
            Qt4Qt5/Qsci/qscilexerverilog.h
            Qt4Qt5/Qsci/qscilexervhdl.h
            Qt4Qt5/Qsci/qscilexerxml.h
            Qt4Qt5/Qsci/qscilexeryaml.h
            Qt4Qt5/Qsci/qscimacro.h
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
            PrintSupport
            Widgets
    )
ENDIF()
