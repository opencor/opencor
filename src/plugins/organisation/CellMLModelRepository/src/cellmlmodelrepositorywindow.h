#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

#include "dockwidget.h"

namespace Ui {
    class CellmlModelRepositoryWindow;
}

namespace OpenCOR {
namespace CellMLModelRepository {

class CellmlModelRepositoryWidget;

class CellmlModelRepositoryWindow : public Core::DockWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindow(QWidget *pParent = 0);
    ~CellmlModelRepositoryWindow();

    virtual void retranslateUi();

private:
    Ui::CellmlModelRepositoryWindow *mUi;

    QStringList mModelNames;
    QStringList mModelUrls;

    CellmlModelRepositoryWidget *mCellmlModelRepositoryWidget;

    QStringList mModelList;

    void outputModelList(const QStringList &pModelList);

private slots:
    void on_nameValue_textChanged(const QString &text);
};

} }

#endif
