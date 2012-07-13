//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATAEDITDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATAEDITDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataEditDetailsWidget;
}

//==============================================================================

class QComboBox;
class QFormLayout;
class QGridLayout;
class QLineEdit;
class QNetworkAccessManager;
class QNetworkReply;
class QStackedWidget;
class QVBoxLayout;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget : public QScrollArea,
                                                      public Core::CommonWidget
{
    Q_OBJECT

private:
    struct Item
    {
        QString resource;
        QString id;
        QString name;
    };

    typedef QList<Item> Items;

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataEditDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const Items &pItems, const QString &pErrorMsg);

private:
    Ui::CellmlAnnotationViewMetadataEditDetailsWidget *mGui;

    QStackedWidget *mWidget;

    QWidget *mMainWidget;
    QVBoxLayout *mMainLayout;

    QWidget *mFormWidget;
    QFormLayout *mFormLayout;

    QStackedWidget *mItemsWidget;

    QWidget *mGridWidget;
    QGridLayout *mGridLayout;

    QNetworkAccessManager *mNetworkAccessManager;

    QComboBox *mQualifierValue;

    QString mTerm;
    QString mTermUrl;
    QString mOtherTermUrl;

    Items mItems;
    QString mErrorMsg;

    void updateItemsGui(const Items &pItems, const QString &pErrorMsg);

    static Item item(const QString &pResource, const QString &pId,
                     const QString &pName);

Q_SIGNALS:
    void guiPopulated(QComboBox *pQualifierValue, QLineEdit *pTermValue);

private Q_SLOTS:
    void lookupTerm(const QString &pTerm);
    void finished(QNetworkReply *pNetworkReply);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
