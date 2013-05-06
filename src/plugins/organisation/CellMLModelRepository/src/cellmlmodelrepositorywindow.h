//==============================================================================
// CellML Model Repository window
//==============================================================================

#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

namespace Ui {
    class CellMLModelRepositoryWindow;
}

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepository {

//==============================================================================

class CellMLModelRepositoryWidget;

//==============================================================================

class CellMLModelRepositoryWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit CellMLModelRepositoryWindow(QWidget *pParent = 0);
    ~CellMLModelRepositoryWindow();

    virtual void retranslateUi();

private:
    Ui::CellMLModelRepositoryWindow *mGui;

    QStringList mModelNames;
    QStringList mModelUrls;

    CellMLModelRepositoryWidget *mCellMLModelRepositoryWidget;

    QStringList mModelList;

    QNetworkAccessManager *mNetworkAccessManager;
    QString mErrorMsg;

    void outputModelList(const QStringList &pModelList);

private Q_SLOTS:
    void on_filterValue_textChanged(const QString &text);
    void on_actionCopy_triggered();
    void on_refreshButton_clicked();

    void finished(QNetworkReply *pNetworkReply);
    void showCustomContextMenu(const QPoint &) const;
};

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
