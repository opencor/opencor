//==============================================================================
// CellML annotation view lists widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWLISTSWIDGET_H
#define CELLMLANNOTATIONVIEWLISTSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewListsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlListWidget;
class CellmlAnnotationViewMetadataListWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewListsWidget : public QSplitter,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewListsWidget(CellmlAnnotationViewWidget *pParent,
                                             CellMLSupport::CellmlFile *pCellmlFile);
    ~CellmlAnnotationViewListsWidget();

    virtual void retranslateUi();

    CellmlAnnotationViewCellmlListWidget * cellmlList() const;
    CellmlAnnotationViewMetadataListWidget * metadataList() const;

private:
    Ui::CellmlAnnotationViewListsWidget *mGui;

    CellmlAnnotationViewCellmlListWidget *mCellmlList;
    CellmlAnnotationViewMetadataListWidget *mMetadataList;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);

private Q_SLOTS:
    void emitSplitterMoved();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
