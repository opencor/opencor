#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

#include "organisationwidget.h"

#include <QNetworkReply>

namespace Ui {
    class CellmlModelRepositoryWindow;
}

namespace OpenCOR {
namespace CellMLModelRepository {

class CellmlModelRepositoryWidget;

class CellmlModelRepositoryWindow : public Core::OrganisationWidget
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

    QString mErrorMsg;

    void outputModelList(const QStringList &pModelList);

private slots:
    void on_nameValue_textChanged(const QString &text);
    void on_actionCopy_triggered();

    void finished(QNetworkReply *pNetworkReply);
    void customContextMenu(const QPoint &) const;
};

} }

#endif
