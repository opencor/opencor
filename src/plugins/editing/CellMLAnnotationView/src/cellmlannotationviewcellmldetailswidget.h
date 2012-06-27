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

    CellmlAnnotationViewCellmlElementDetailsWidget *mCellmlElementDetails;
    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

    QString mQualifierInformationTemplate;

    QString mModelQualifierSvg;
    QString mBiologyQualifierSvg;

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
