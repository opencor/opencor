//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H

//==============================================================================

#include "cellmlannotationviewcellmlelementdetailswidget.h"
#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;
class QWebView;

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlDetailsWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewCellmlDetailsWidget : public QSplitter,
                                                public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewCellmlDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewCellmlDetailsWidget *mGui;

    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebView;

    CellmlAnnotationViewCellmlElementDetailsWidget *mCellmlElementDetails;
    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);

private Q_SLOTS:
    void emitSplitterMoved();

    void newCmetaId(const QString &pCmetaId);

    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void resourceIdLookupRequested(const QString &pResource,
                                   const QString &pId,
                                   const bool &pRetranslate);
    void unknownLookupRequested();

    void metadataUpdated();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
