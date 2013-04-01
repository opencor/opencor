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
class QLabel;
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

    Ui::CellmlAnnotationViewMetadataEditDetailsWidget *mGui;

    QStackedWidget *mWidget;

    QWidget *mMainWidget;
    QVBoxLayout *mMainLayout;

    QWidget *mFormWidget;
    QFormLayout *mFormLayout;

    QScrollArea *mItemsScrollArea;

    QWidget *mGridWidget;
    QGridLayout *mGridLayout;

    QNetworkAccessManager *mNetworkAccessManager;

    QComboBox *mQualifierValue;
    QPushButton *mLookupQualifierButton;

    int mQualifierIndex;
    bool mLookupQualifierButtonIsChecked;

    QLineEdit *mTermValue;
    QPushButton *mAddTermButton;

    QString mTerm;
    bool mTermIsDirect;

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

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    QLabel *mCurrentResourceOrIdLabel;

    void updateGui(const Items &pItems, const QString &pErrorMsg,
                   const bool &pLookupTerm,
                   const int &pItemsVerticalScrollBarPosition,
                   const bool &pRetranslate);
    void updateItemsGui(const Items &pItems, const QString &pErrorMsg,
                        const bool &pLookupTerm);

    static Item item(const QString &pName,
                     const QString &pResource, const QString &pId);

    void genericLookup(const QString &pItemInformation = QString(),
                       const Type &pType = No,
                       const bool &pRetranslate = false);

Q_SIGNALS:
    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void idLookupRequested(const QString &pResource, const QString &pId,
                           const bool &pRetranslate);
    void noLookupRequested();

    void rdfTripleAdded(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   const bool &pUpdateItemsGui = true);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void disableLookupInformation();

    void qualifierChanged(const int &pQualifierIndex);
    void qualifierChanged(const QString &pQualifier);

    void lookupQualifier();
    void lookupResource(const QString &pItemInformation);
    void lookupId(const QString &pItemInformation);

    void termChanged(const QString &pTerm);
    void termLookedUp(QNetworkReply *pNetworkReply);

    void addTerm();
    void addRetrievedTerm();

    void trackItemsVerticalScrollBarPosition(const int &pPosition);

    void showCustomContextMenu(const QPoint &pPosition);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
