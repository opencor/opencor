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
class QPushButton;
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
        QString name;
        QString resource;
        QString id;

        bool operator<(const Item &pItem) const;
    };

    typedef QList<Item> Items;

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataEditDetailsWidget();

    virtual void retranslateUi();

private:
    enum Type {
        No,
        Qualifier,
        Resource,
        Id
    };

    CellmlAnnotationViewWidget *mParent;
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewMetadataEditDetailsWidget *mGui;

    QStackedWidget *mWidget;

    QWidget *mMainWidget;
    QVBoxLayout *mMainLayout;

    QWidget *mFormWidget;
    QFormLayout *mFormLayout;

    QLineEdit *mTermValue;

    QScrollArea *mItemsScrollArea;

    QWidget *mGridWidget;
    QGridLayout *mGridLayout;

    QNetworkAccessManager *mNetworkAccessManager;

    QComboBox *mQualifierValue;
    QPushButton *mLookupButton;
    bool mLookupButtonIsChecked;

    QString mTerm;
    QString mTermUrl;
    QString mOtherTermUrl;

    Items mItems;
    QString mErrorMsg;
    bool mLookupTerm;

    QString mInformation;
    Type mType;

    bool mLookupInformation;

    QMap<QObject *, Item> mItemsMapping;

    int mItemsVerticalScrollBarPosition;

    void updateGui(const Items &pItems, const QString &pErrorMsg,
                   const bool &pLookupTerm = false,
                   const int &pItemsVerticalScrollBarPosition = 0,
                   const bool &pRetranslate = false);
    void updateItemsGui(const Items &pItems, const QString &pErrorMsg,
                        const bool &pLookupTerm);

    static Item item(const QString &pName,
                     const QString &pResource, const QString &pId);

    void genericLookup(const QString &pInformation = QString(),
                       const Type &pType = No,
                       const bool &pRetranslate = false);

Q_SIGNALS:
    void guiPopulated(QComboBox *pQualifierValue, QPushButton *pLookupButton,
                      QLineEdit *pTermValue);

    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void idLookupRequested(const QString &pResource, const QString &pId,
                           const bool &pRetranslate);
    void noLookupRequested();

    void metadataAdded(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

private Q_SLOTS:
    void disableLookupInformation();

    void qualifierChanged(const QString &pQualifier);

    void lookupQualifier();
    void lookupResource(const QString &pInformation);
    void lookupId(const QString &pInformation);

    void lookupTerm(const QString &pTerm);
    void termLookupFinished(QNetworkReply *pNetworkReply);

    void addRdfTriple();

    void trackItemsVerticalScrollBarPosition(const int &pPosition);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
