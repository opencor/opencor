//==============================================================================
// Single cell simulation view contents widget
//==============================================================================

#include "borderedwidget.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewgraphpanelswidget.h"
#include "singlecellsimulationviewinformationwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewContentsWidget::SingleCellSimulationViewContentsWidget(QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mSplitterSizes(QList<int>())
{
    // Keep track of our movement
    // Note: we need to keep track of our movement so that saveSettings() can
    //       work fine even when we are not visible (which happens when a CellML
    //       file cannot be run for some reason or another)...

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterMoved()));

    // Create our information widget

    mInformationWidget = new SingleCellSimulationViewInformationWidget(this);

    mInformationWidget->setObjectName("Information");

    // Create our graph panels widget

    mGraphPanelsWidget = new SingleCellSimulationViewGraphPanelsWidget(this);

    mGraphPanelsWidget->setObjectName("GraphPanels");

    // Add our information and graph panels widgets to ourselves

    addWidget(new Core::BorderedWidget(mInformationWidget,
                                       false, false, true, true));
    addWidget(new Core::BorderedWidget(mGraphPanelsWidget,
                                       false, true, true, false));

    // Make our graph panels widget our focus proxy

    setFocusProxy(mGraphPanelsWidget);
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::retranslateUi()
{
    // Retranslate our information widget

    mInformationWidget->retranslateUi();
}

//==============================================================================

static const QString SettingsContentsCount = "ContentsCount";
static const QString SettingsContentsSize  = "ContentsSize%1";

//==============================================================================

void SingleCellSimulationViewContentsWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes

    int sizesCount = pSettings->value(SettingsContentsCount, 0).toInt();
    mSplitterSizes = QList<int>();

    if (!sizesCount)
        // There are no previous sizes, so get some default ones

        mSplitterSizes << 0.25*qApp->desktop()->screenGeometry().width()
                       << 0.75*qApp->desktop()->screenGeometry().width();
    else
        for (int i = 0; i < sizesCount; ++i)
            mSplitterSizes << pSettings->value(SettingsContentsSize.arg(i)).toInt();

    setSizes(mSplitterSizes);

    // Retrieve the settings of our information and graph panels widgets

    pSettings->beginGroup(mInformationWidget->objectName());
        mInformationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsContentsCount, mSplitterSizes.count());

    for (int i = 0, iMax = mSplitterSizes.count(); i < iMax; ++i)
        pSettings->setValue(SettingsContentsSize.arg(i), mSplitterSizes[i]);

    // Keep track of the settings of our information and graph panels widgets

    pSettings->beginGroup(mInformationWidget->objectName());
        mInformationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

SingleCellSimulationViewInformationWidget * SingleCellSimulationViewContentsWidget::informationWidget()
{
    // Return our information widget

    return mInformationWidget;
}

//==============================================================================

SingleCellSimulationViewGraphPanelsWidget * SingleCellSimulationViewContentsWidget::graphPanelsWidget()
{
    // Return our graph panels widget

    return mGraphPanelsWidget;
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::splitterMoved()
{
    // Our splitter has been moved, so keep track of its new sizes

    mSplitterSizes = sizes();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
