//==============================================================================
// CellML Model Repository window
//==============================================================================

#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

namespace Ui {
    class CellmlModelRepositoryWindow;
}

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepository {

//==============================================================================

class CellmlModelRepositoryWidget;

//==============================================================================

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

    QNetworkAccessManager *mNetworkAccessManager;
    QString mErrorMsg;

    void outputModelList(const QStringList &pModelList);

private Q_SLOTS:
    void on_nameValue_textChanged(const QString &text);
    void on_actionCopy_triggered();
    void on_refreshButton_clicked();

    void finished(QNetworkReply *pNetworkReply);
    void customContextMenu(const QPoint &) const;
};

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
