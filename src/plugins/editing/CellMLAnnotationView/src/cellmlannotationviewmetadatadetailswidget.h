//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

class QSplitter;
class QWebView;

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataDetailsWidget;
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

class CellmlAnnotationViewMetadataEditDetailsWidget;
class CellmlAnnotationViewMetadataViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataDetailsWidget();

    virtual void retranslateUi();

    QSplitter * splitter() const;

    CellmlAnnotationViewMetadataEditDetailsWidget * metadataEditDetails() const;
    CellmlAnnotationViewMetadataViewDetailsWidget * metadataViewDetails() const;

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataDetailsWidget *mGui;

    Core::BorderedWidget *mBorderedUnsupportedMetadataMsg;
    Core::UserMessageWidget *mUnsupportedMetadataMsg;

    QSplitter *mSplitter;

    Core::BorderedWidget *mBorderedMetadataEditDetails;
    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebView;

    CellmlAnnotationViewMetadataEditDetailsWidget *mMetadataEditDetails;
    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

private Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);
    void emitSplitterMoved();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

    void lookupQualifier(const QString &pQualifier, const bool &pRetranslate);
    void lookupResource(const QString &pResource, const bool &pRetranslate);
    void lookupId(const QString &pResource, const QString &pId,
                  const bool &pRetranslate);
    void lookupNothing();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
