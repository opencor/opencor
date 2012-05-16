//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewWidget;
}

//==============================================================================

class QSplitter;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewListsWidget;
class CellmlAnnotationViewDetailsWidget;

//==============================================================================

class CellmlAnnotationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        const QString &pFileName);
    ~CellmlAnnotationViewWidget();

    virtual void retranslateUi();

    QList<int> horizontalSplitterSizes() const;

    CellMLSupport::CellmlFile * cellmlFile() const;

    CellmlAnnotationViewListsWidget * listsWidget() const;
    CellmlAnnotationViewDetailsWidget * detailsWidget() const;

private:
    Ui::CellmlAnnotationViewWidget *mGui;

    CellMLSupport::CellmlFile *mCellmlFile;

    QSplitter *mHorizontalSplitter;

    CellmlAnnotationViewListsWidget *mListsWidget;
    CellmlAnnotationViewDetailsWidget *mDetailsWidget;

Q_SIGNALS:
    void horizontalSplitterMoved(const QList<int> &pHorizontalSizes);

public Q_SLOTS:
    void updateHorizontalSplitter(const QList<int> &pHorizontalSizes);

private Q_SLOTS:
    void emitHorizontalSplitterMoved();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
