//==============================================================================
// Single cell view contents widget
//==============================================================================

#ifndef SINGLECELLVIEWCONTENTSWIDGET_H
#define SINGLECELLVIEWCONTENTSWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelsWidget;
class SingleCellViewInformationWidget;

//==============================================================================

class SingleCellViewContentsWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewContentsWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellViewInformationWidget * informationWidget();
    SingleCellViewGraphPanelsWidget * graphPanelsWidget();

private:
    QList<int> mSplitterSizes;

    SingleCellViewInformationWidget *mInformationWidget;
    SingleCellViewGraphPanelsWidget *mGraphPanelsWidget;

private Q_SLOTS:
    void splitterMoved();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
