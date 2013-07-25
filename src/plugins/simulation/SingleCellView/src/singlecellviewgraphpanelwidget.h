//==============================================================================
// Single cell view graph panel widget
//==============================================================================

#ifndef SINGLECELLVIEWGRAPHPANELWIDGET_H
#define SINGLECELLVIEWGRAPHPANELWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

class QFrame;

//==============================================================================

namespace Ui {
    class SingleCellViewGraphPanelWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelPlotWidget;

//==============================================================================

class SingleCellViewGraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellViewGraphPanelWidget(QWidget *pParent = 0);
    ~SingleCellViewGraphPanelWidget();

    bool isActive() const;
    void setActive(const bool &pActive);

    QList<SingleCellViewGraphPanelPlotGraph *> graphs() const;

    void addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

    void replot();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    Ui::SingleCellViewGraphPanelWidget *mGui;

    QFrame *mMarker;
    SingleCellViewGraphPanelPlotWidget *mPlot;

    bool mActive;

    void updateMarkerColor();

Q_SIGNALS:
    void activated(SingleCellViewGraphPanelWidget *pGraphPanel);
    void inactivated(SingleCellViewGraphPanelWidget *pGraphPanel);

    void graphAdded(SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphRemoved(SingleCellViewGraphPanelPlotGraph *pGraph);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
