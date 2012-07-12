//==============================================================================
// CellML annotation view CellML metadata details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLMETADATADETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLMETADATADETAILSWIDGET_H

//==============================================================================

#include "cellmlannotationviewcellmlelementdetailswidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

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
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewCellmlMetadataDetailsWidget : public QSplitter,
                                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                             CellMLSupport::CellmlFile *pCellmlFile);
    ~CellmlAnnotationViewCellmlMetadataDetailsWidget();

    virtual void retranslateUi();

    CellmlAnnotationViewMetadataViewDetailsWidget *metadataViewDetails() const;

private:
    CellmlAnnotationViewWidget *mParent;
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewCellmlMetadataDetailsWidget *mGui;

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
