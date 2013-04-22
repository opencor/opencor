//==============================================================================
// Single cell view information widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONWIDGET_H
#define SINGLECELLVIEWINFORMATIONWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class SingleCellViewInformationWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class CollapsibleWidget;
}   // namespace Core

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewInformationParametersWidget;
class SingleCellViewInformationSimulationWidget;
class SingleCellViewInformationSolversWidget;

//==============================================================================

class SingleCellViewInformationWidget : public QScrollArea,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationWidget(QWidget *pParent = 0);
    ~SingleCellViewInformationWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellViewInformationSimulationWidget * simulationWidget();
    SingleCellViewInformationSolversWidget * solversWidget();
    SingleCellViewInformationParametersWidget * parametersWidget();

    void finishEditing();

private:
    Ui::SingleCellViewInformationWidget *mGui;

    Core::CollapsibleWidget *mCollapsibleWidget;

    SingleCellViewInformationSimulationWidget *mSimulationWidget;
    SingleCellViewInformationSolversWidget *mSolversWidget;
    SingleCellViewInformationParametersWidget *mParametersWidget;
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
