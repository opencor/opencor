//==============================================================================
// RawCellMLView plugin
//==============================================================================

#include "cellmlmodel.h"
#include "cellmlsupportplugin.h"
#include "cellmlsupportglobal.h"
#include "commonwidget.h"
#include "qscintilla.h"
#include "rawcellmlviewplugin.h"

//==============================================================================

#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QTextStream>
#include <QUrl>

#include <QDebug>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

PLUGININFO_FUNC RawCellMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using an XML editor");
    descriptions.insert("fr", "Une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide d'un éditeur XML");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      true,
                      QStringList() << "CoreCellMLEditing" << "QScintillaSupport",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(RawCellMLView, RawCellMLViewPlugin)

//==============================================================================

RawCellMLViewPlugin::RawCellMLViewPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

QWidget * RawCellMLViewPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (QFileInfo(pFileName).suffix().compare(CellMLSupport::CellmlFileExtension))
        // Not the expected file extension, so...

        return GuiInterface::newViewWidget(pFileName);



//--- TESTING --- BEGIN ---

    qDebug("=======================================");
    qDebug("%s:", pFileName.toLatin1().constData());

    // Load the CellML model

    CellMLSupport::CellmlModel *cellmlModel = new CellMLSupport::CellmlModel(pFileName);

    // Check the model's validity

    if (cellmlModel->isValid()) {
        // The model is valid, but let's see whether warnings were generated

        int nbOfWarnings = cellmlModel->issues().count();

        if (nbOfWarnings)
            qDebug(" - The model was properly loaded:");
        else
            qDebug(" - The model was properly loaded.");
    } else {
        qDebug(" - The model was NOT properly loaded:");
    }

    // Output any warnings/errors that were generated

    foreach (const CellMLSupport::CellmlModelIssue &cellmlModelIssue,
             cellmlModel->issues()) {
        QString type = QString((cellmlModelIssue.type() == CellMLSupport::CellmlModelIssue::Error)?"Error":"Warrning");
        QString message = cellmlModelIssue.formattedMessage();
        uint32_t line = cellmlModelIssue.line();
        uint32_t column = cellmlModelIssue.column();
        QString importedModel = cellmlModelIssue.importedModel();

        if (line && column) {
            if (importedModel.isEmpty())
                qDebug("    [%s at line %s column %s] %s", type.toLatin1().constData(),
                                                           QString::number(cellmlModelIssue.line()).toLatin1().constData(),
                                                           QString::number(cellmlModelIssue.column()).toLatin1().constData(),
                                                           message.toUtf8().constData());
            else
                qDebug("    [%s at line %s column %s from imported model %s] %s", type.toLatin1().constData(),
                                                                                  QString::number(cellmlModelIssue.line()).toLatin1().constData(),
                                                                                  QString::number(cellmlModelIssue.column()).toLatin1().constData(),
                                                                                  importedModel.toLatin1().constData(),
                                                                                  message.toUtf8().constData());
        } else {
            if (importedModel.isEmpty())
                qDebug("    [%s] %s", type.toLatin1().constData(),
                                      message.toUtf8().constData());
            else
                qDebug("    [%s from imported model %s] %s", type.toLatin1().constData(),
                                                             importedModel.toLatin1().constData(),
                                                             message.toUtf8().constData());
        }
    }

    // Get a runtime for the model

    CellMLSupport::CellmlModelRuntime *cellmlModelRuntime = cellmlModel->runtime();

    if (cellmlModelRuntime->isValid()) {
        qDebug(" - The model's runtime was properly generated.");
        qDebug("    [Information] Model type = %s", (cellmlModelRuntime->modelType() == CellMLSupport::CellmlModelRuntime::Ode)?"ODE":"DAE");
    } else {
        qDebug(" - The model's runtime was NOT properly generated:");

        foreach (const CellMLSupport::CellmlModelIssue &cellmlModelIssue,
                 cellmlModelRuntime->issues()) {
            QString type = QString((cellmlModelIssue.type() == CellMLSupport::CellmlModelIssue::Error)?"Error":"Warrning");
            QString message = cellmlModelIssue.formattedMessage();

            qDebug("    [%s] %s", type.toLatin1().constData(),
                                  message.toUtf8().constData());
        }
    }

    // Done with our testing, so...

    delete cellmlModel;

//--- TESTING --- END ---



    // Create, set up and return a raw CellML Scintilla editor

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        // For some reason, the file couldn't be opened, so...

        return GuiInterface::newViewWidget(pFileName);

    // The file was properly opened, so create a Scintilla editor and associate
    // an XML (i.e. raw CellML) lexer to it

    QsciScintilla *res = new QScintillaSupport::QScintilla(QTextStream(&file).readAll(),
                                                           !(QFileInfo(pFileName).isWritable()),
                                                           new QsciLexerXML(mMainWindow),
                                                           mMainWindow);

    // We are done with the file, so close it

    file.close();

    // Our raw Scintilla editor is now ready, so...

    return res;
}

//==============================================================================

QString RawCellMLViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("Raw CellML");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
