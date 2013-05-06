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
    class CellMLAnnotationViewMetadataDetailsWidget;
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

class CellMLAnnotationViewMetadataEditDetailsWidget;
class CellMLAnnotationViewMetadataViewDetailsWidget;
class CellMLAnnotationViewWidget;

//==============================================================================

class CellMLAnnotationViewMetadataDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellMLAnnotationViewMetadataDetailsWidget(CellMLAnnotationViewWidget *pParent);
    ~CellMLAnnotationViewMetadataDetailsWidget();

    virtual void retranslateUi();

    void addRdfTriple(CellMLSupport::CellMLFileRdfTriple *pRdfTriple);

    QSplitter * splitter() const;

    CellMLAnnotationViewMetadataEditDetailsWidget * metadataEditDetails() const;
    CellMLAnnotationViewMetadataViewDetailsWidget * metadataViewDetails() const;

private:
    CellMLAnnotationViewWidget *mParent;

    Ui::CellMLAnnotationViewMetadataDetailsWidget *mGui;

    Core::BorderedWidget *mBorderedCategoryMessage;
    Core::UserMessageWidget *mCategoryMessage;

    Core::BorderedWidget *mBorderedUnsupportedMetadataMessage;
    Core::UserMessageWidget *mUnsupportedMetadataMessage;

    QSplitter *mSplitter;

    Core::BorderedWidget *mBorderedMetadataEditDetails;
    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebView;

    CellMLAnnotationViewMetadataEditDetailsWidget *mMetadataEditDetails;
    CellMLAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

    CellMLSupport::CellMLFile *mCellMLFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement);

private Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);
    void emitSplitterMoved();

    void lookupQualifier(const QString &pQualifier, const bool &pRetranslate);
    void lookupResource(const QString &pResource, const bool &pRetranslate);
    void lookupId(const QString &pResource, const QString &pId,
                  const bool &pRetranslate);
    void lookupNothing();

    void removeAllMetadata();

    void updateMetadataEditDetails();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
