project(BondGraphUIWidgetsPlugin)

# # Load miriam_terms.json to file variable
# include(../../../../cmake/vtkEncodeString.cmake)
# set(MIRIAMTERMSJSON miriamtermsjsonstring)
# vtk_encode_string(
#   INPUT "res/miriam/miriam_terms.json"
#   NAME ${MIRIAMTERMSJSON}
#   SOURCE_OUTPUT MIRIAMTERMSJSONSOURCE
#   BINARY
# )

# Add the plugin
if(WIN32)
    set(PACKAGE_C_FLAGS "/wd4005 /wd4244 /wd4267 /wd4311 /wd4312")
    set(PACKAGE_CXX_FLAGS "/wd4244 /wd4267 /wd4309 /wd4477 /wd4530 /wd4577")
elseif(NOT APPLE)
    set(PACKAGE_CXX_FLAGS "-std=c++0x -Wno-deprecated -Wno-misleading-indentation -Wno-reorder -Wno-unused-function")
endif()


add_plugin(BondGraphUIWidgets
    SOURCES
        ../../plugininfo.cpp
        src/bgconnection.cpp
        src/bgdconnection.cpp
        src/bgdiffundomanager.cpp
        src/bgeditorsceneactions.cpp
        src/bgeditorscene.cpp
        src/bgeditorview.cpp
        src/bgelement.cpp
        src/bgelementconnectionpropertiesui.cpp
        src/bgannotationmiriamtermsui.cpp
        src/bgannotationviewmetadataeditdetailsui.cpp
        src/bgelementeditorscene.cpp
        src/bgelementeditoruicontroller.cpp
        src/bgelementsceneactions.cpp
        src/bgitemattribute.cpp
        src/bgparameterdelegate.cpp
        src/bgparameterimportdialog.cpp
        src/bgport.cpp
        src/bgjsonmodels.cpp
        src/bgexportutilities.cpp
        src/bgimageexport.cpp
        src/bgprojectdetails.cpp
        src/bgimageexportdialog.cpp
        src/bgbasicelementannotationtable.cpp
        src/bguserdefinedimportdialog.cpp
        src/bgelementproxymapdialog.cpp
        src/bgcolorschemesuicontroller.cpp
        src/bgconnectionstable.cpp
        src/bgporteditordialog.cpp
        src/controlpoint.cpp
        src/bgeditorsearchdialog.cpp
        src/bgtextlabeledit.cpp
        src/bgtransformrect.cpp
        src/sceneitem.cpp
        src/scenemenucontroller.cpp
        src/sceneoptionsdialog.cpp
        src/sceneundomanager.cpp
        src/utils.cpp
        src/bondgraphuiwidget.cpp
        #Widgetbox
        src/thirdparty/widgetbox/widgetbox.cpp
        src/thirdparty/widgetbox/categorybutton.cpp
        src/thirdparty/widgetbox/pageeventfilter.cpp
        src/thirdparty/widgetbox/categorywidgets.cpp
        src/thirdparty/widgetbox/libraryitem.cpp
        #Accordian
        src/thirdparty/qtaccordian/clickableframe.cpp
        src/thirdparty/qtaccordian/contentpane.cpp
        src/thirdparty/qtaccordian/qaccordion.cpp
        #jkqtLatex
        src/thirdparty/jkqtlatex/jkqtmathtext.cpp
        src/thirdparty/jkqtlatex/jkqtpcodestructuring.cpp
        src/thirdparty/jkqtlatex/jkqtpstringtools.cpp
        #qsint-widgets
        src/thirdparty/qsintwidgets/colorbutton.cpp
        src/thirdparty/qsintwidgets/colorcombobox.cpp
        src/thirdparty/qsintwidgets/colordefs.cpp
        src/thirdparty/qsintwidgets/colorgrid.cpp
        src/thirdparty/qsintwidgets/doublespincombobox.cpp
        src/thirdparty/qsintwidgets/ledwidget.cpp
        src/thirdparty/qsintwidgets/pathpicker.cpp
        src/thirdparty/qsintwidgets/qfontbutton.cpp
        src/thirdparty/qsintwidgets/qmenubutton.cpp
        src/thirdparty/qsintwidgets/qpenbutton.cpp
        src/thirdparty/qsintwidgets/qpencombobox.cpp
        src/thirdparty/qsintwidgets/qsplitbutton.cpp
        src/thirdparty/qsintwidgets/roundprogressbar.cpp
        src/thirdparty/qsintwidgets/slider2d.cpp
        src/thirdparty/qsintwidgets/slider.cpp
        src/thirdparty/qsintwidgets/spincombobox.cpp
        src/thirdparty/qsintwidgets/spinslider.cpp
        src/thirdparty/qsintwidgets/splitbutton.cpp
        src/thirdparty/qsintwidgets/timepicker.cpp

        #Generated using vtkEncodeString.cmake - hardcoded to integrate with OpenCOR without adding vtkEncodeString.cmake to OpenCOR
        src/miriamtermsjsonstring.cpp

        src/thirdparty/widgetbox/widgetboxicons.qrc
        res/resources.qrc
        src/thirdparty/qtaccordian/icons/qaccordionicons.qrc
        src/bondgraphuiwidgetsplugin.cpp
    UIS 
        src/BGAnnotationViewMetadataEditDetailsUI.ui
        src/BGAttributesEditorUI.ui
        src/BGBasicElementAnnotationTable.ui
        src/BGConnectionsTable.ui
        src/BGEditorSearchDialog.ui
        src/BGElementAttributesEditorUI.ui
        src/BGElementConnectionPropertiesUI.ui
        src/BGImageExportDialog.ui
        src/BGParameterImport.ui
        src/BGPortEditorDialog.ui
        src/BGProjectDetails.ui
        src/BGUserdefinedImportDialog.ui
        src/SceneOptionsDialog.ui       
    QT_MODULES
        Widgets
        Gui
        Network
        OpenGL  
        Xml
        PrintSupport
        Core
    PLUGINS
        BondGraphAPI
)
