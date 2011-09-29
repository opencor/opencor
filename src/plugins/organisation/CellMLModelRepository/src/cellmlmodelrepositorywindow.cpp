#include "cellmlmodelrepositorywindow.h"
#include "cellmlmodelrepositorywidget.h"
#include "internetfile.h"

#include "ui_cellmlmodelrepositorywindow.h"

#include <QJsonParser>

#include <QDebug>

namespace OpenCOR {
namespace CellMLModelRepository {

CellmlModelRepositoryWindow::CellmlModelRepositoryWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::CellmlModelRepositoryWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the CellML Model Repository widget

    mCellmlModelRepositoryWidget = new CellmlModelRepositoryWidget("CellmlModelRepositoryWidget",
                                                                   this);

    mUi->dockWidgetContents->layout()->addWidget(mCellmlModelRepositoryWidget);

    // Retrieve the list of models in the CellML Model Repository as JSON code
    // from http://50.18.64.32/workspace/rest/contents.json

    Core::InternetFile internetFile;

    QNetworkReply *networkReply = internetFile.get("http://50.18.64.32/workspace/rest/contents.json");

    // Output the list of models, should we have retrieved it without any
    // problem

    if (networkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJson::Parser jsonParser;
        bool parsingOk;

        QVariantMap res = jsonParser.parse (networkReply->readAll(), &parsingOk).toMap();

        if (parsingOk) {
            // Retrieve the name of the keys

            QStringList keys;

            foreach (const QVariant &keyVariant, res["keys"].toList())
                keys.append(keyVariant.toString());

            // Retrieve the list of models itself

            QStringList modelNames;
            QStringList modelUrls;

            foreach(const QVariant &modelVariant, res["values"].toList()) {
                QVariantList modelDetailsVariant = modelVariant.toList();

                modelNames.append(modelDetailsVariant.at(0).toString());
                modelUrls.append(modelDetailsVariant.at(1).toString());
            }

            // Output the list of models

            for (int i = 0; i < modelNames.count(); ++i) {
                qDebug("--------------------");
                qDebug("Name: %s", modelNames.at(i).toLatin1().constData());
                qDebug("URL:  %s", modelUrls.at(i).toLatin1().constData());
            }
        }
    }
}

CellmlModelRepositoryWindow::~CellmlModelRepositoryWindow()
{
    // Delete the UI

    delete mUi;
}

void CellmlModelRepositoryWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);
}

} }
