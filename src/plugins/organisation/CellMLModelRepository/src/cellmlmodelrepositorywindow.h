#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

#include "dockwidget.h"

#include <QEventLoop>
#include <QNetworkReply>

namespace Ui {
    class CellmlModelRepositoryWindow;
}

namespace OpenCOR {
namespace CellMLModelRepository {

class CellmlModelRepositoryWindow : public Core::DockWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindow(QWidget *pParent = 0);
    ~CellmlModelRepositoryWindow();

    virtual void retranslateUi();

private:
    Ui::CellmlModelRepositoryWindow *mUi;
};

} }

#endif
