#include "cellmlmodelrepositorywindow.h"
#include "internetfile.h"

#include "ui_cellmlmodelrepositorywindow.h"

#include <QDebug>

namespace OpenCOR {
namespace CellMLModelRepository {

CellmlModelRepositoryWindow::CellmlModelRepositoryWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::CellmlModelRepositoryWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Retrieve the list of models in the CellML Model Repository as JSON code
    // from http://50.18.64.32/workspace/res/contents.json

    Core::InternetFile internetFile;

    QNetworkReply *res = internetFile.get("http://50.18.64.32/workspace/rest/contents.json");

    qDebug("%s", QString(res->readAll()).toLatin1().constData());
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
