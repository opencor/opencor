//==============================================================================
// CellML annotation view CellML metadata details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLMETADATADETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLMETADATADETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

class QSplitter;
class QWebView;

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlMetadataDetailsWidget;
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

class CellmlAnnotationViewCellmlMetadataDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewCellmlMetadataDetailsWidget();

    virtual void retranslateUi();

    QSplitter * splitter() const;

    CellmlAnnotationViewMetadataViewDetailsWidget *metadataViewDetails() const;

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewCellmlMetadataDetailsWidget *mGui;

    Core::BorderedWidget *mBorderedUnsupportedMetadataMsg;
    Core::UserMessageWidget *mUnsupportedMetadataMsg;

    QSplitter *mSplitter;

    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebView;

    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

private Q_SLOTS:
    void emitSplitterMoved();

    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void resourceIdLookupRequested(const QString &pResource,
                                   const QString &pId,
                                   const bool &pRetranslate);
    void unknownLookupRequested();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
