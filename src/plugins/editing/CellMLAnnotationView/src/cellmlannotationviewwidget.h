//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfilecomponentref.h"
#include "widget.h"

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewWidget;
}

//==============================================================================

class QSplitter;
class QTextEdit;
class QTreeView;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        const QString &pFileName);

    QList<int> horizontalSplitterSizes() const;

private:
    Ui::CellmlAnnotationViewWidget *mUi;

    QSplitter *mHorizontalSplitter;

    QTreeView *mTreeView;
    QTextEdit *mDebugOutput;

    void initTreeView(const QString &pFileName);
    void initComponentRefTreeView(const QString &pLeadingSpace, CellMLSupport::CellmlFileComponentRef *pComponentRef);

Q_SIGNALS:
    void horizontalSplitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateHorizontalSplitter(const QList<int> &pSizes);

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
