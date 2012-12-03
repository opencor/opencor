//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"
#include "qtpropertybrowserwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

//---GRY---
#include <QDate>
#include <QtVariantPropertyManager>

//==============================================================================

#include "ui_singlecellsimulationviewinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationWidget::SingleCellSimulationViewInformationWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and customise our property editor

    mPropertyEditor = new QtPropertyBrowserSupport::QtPropertyBrowserWidget(this);

    QtVariantPropertyManager *propertyManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(this);

    mPropertyEditor->setAlternatingRowColors(false);
#ifdef Q_WS_MAC
    mPropertyEditor->setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our property editor...
//---GRY--- FOR SOME REASON, IT DOESN'T WORK, BUT WE STILL LEAVE IT HERE FOR
//          NOW...
#endif
    mPropertyEditor->setFactoryForManager(propertyManager, editorFactory);
    mPropertyEditor->setHeaderVisible(false);
    mPropertyEditor->setPropertiesWithoutValueMarked(true);
    mPropertyEditor->setRootIsDecorated(false);

    // Create and populate our simulation section

    mSimulationSection = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString());

    mPropertyEditor->addProperty(mSimulationSection);

    // Create and populate our solvers section

    mSolversSection = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString());

    mPropertyEditor->addProperty(mSolversSection);

    // Create and populate our traces section

    mTracesSection = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString());

    mPropertyEditor->addProperty(mTracesSection);

    // Create and populate our parameters section

    mParametersSection = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString());

    mPropertyEditor->addProperty(mParametersSection);





    // Testing of QtPropertyBrowser
    //---GRY--- THE BELOW IS FOR TESTING PURPOSES ONLY...

    QtProperty *veryTopItem = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String(" Super Group Property"));
    QtProperty *topItem = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String(" Group Property"));

    QtVariantProperty *item = propertyManager->addProperty(QVariant::Bool, QLatin1String(" Bool Property"));
    item->setValue(true);
    veryTopItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Int, QLatin1String(" Int Property"));
    item->setValue(20);
    item->setAttribute(QLatin1String("minimum"), 0);
    item->setAttribute(QLatin1String("maximum"), 100);
    item->setAttribute(QLatin1String("singleStep"), 10);
    veryTopItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Double, QLatin1String(" Double Property"));
    item->setValue(1.2345);
    item->setAttribute(QLatin1String("singleStep"), 0.1);
    item->setAttribute(QLatin1String("decimals"), 3);
    veryTopItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::String, QLatin1String(" String Property"));
    item->setValue("Value");
    veryTopItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::String, QLatin1String(" String Property (Password)"));
    item->setAttribute(QLatin1String("echoMode"), QLineEdit::Password);
    item->setValue("Password");
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Date, QLatin1String(" Date Property"));
    item->setValue(QDate::currentDate().addDays(2));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Time, QLatin1String(" Time Property"));
    item->setValue(QTime::currentTime());
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::DateTime, QLatin1String(" DateTime Property"));
    item->setValue(QDateTime::currentDateTime());
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::KeySequence, QLatin1String(" KeySequence Property"));
    item->setValue(QKeySequence(Qt::ControlModifier | Qt::Key_Q));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Char, QLatin1String(" Char Property"));
    item->setValue(QChar(386));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Locale, QLatin1String(" Locale Property"));
    item->setValue(QLocale(QLocale::Polish, QLocale::Poland));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Point, QLatin1String(" Point Property"));
    item->setValue(QPoint(10, 10));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::PointF, QLatin1String(" PointF Property"));
    item->setValue(QPointF(1.2345, -1.23451));
    item->setAttribute(QLatin1String("decimals"), 3);
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Size, QLatin1String(" Size Property"));
    item->setValue(QSize(20, 20));
    item->setAttribute(QLatin1String("minimum"), QSize(10, 10));
    item->setAttribute(QLatin1String("maximum"), QSize(30, 30));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::SizeF, QLatin1String(" SizeF Property"));
    item->setValue(QSizeF(1.2345, 1.2345));
    item->setAttribute(QLatin1String("decimals"), 3);
    item->setAttribute(QLatin1String("minimum"), QSizeF(0.12, 0.34));
    item->setAttribute(QLatin1String("maximum"), QSizeF(20.56, 20.78));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Rect, QLatin1String(" Rect Property"));
    item->setValue(QRect(10, 10, 20, 20));
    topItem->addSubProperty(item);
    item->setAttribute(QLatin1String("constraint"), QRect(0, 0, 50, 50));

    item = propertyManager->addProperty(QVariant::RectF, QLatin1String(" RectF Property"));
    item->setValue(QRectF(1.2345, 1.2345, 1.2345, 1.2345));
    topItem->addSubProperty(item);
    item->setAttribute(QLatin1String("constraint"), QRectF(0, 0, 50, 50));
    item->setAttribute(QLatin1String("decimals"), 3);

    item = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(),
                    QLatin1String(" Enum Property"));
    QStringList enumNames;
    enumNames << "Enum0" << "Enum1" << "Enum2";
    item->setAttribute(QLatin1String("enumNames"), enumNames);
    item->setValue(1);
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QtVariantPropertyManager::flagTypeId(),
                    QLatin1String(" Flag Property"));
    QStringList flagNames;
    flagNames << "Flag0" << "Flag1" << "Flag2";
    item->setAttribute(QLatin1String("flagNames"), flagNames);
    item->setValue(5);
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::SizePolicy, QLatin1String(" SizePolicy Property"));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Font, QLatin1String(" Font Property"));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Cursor, QLatin1String(" Cursor Property"));
    topItem->addSubProperty(item);

    item = propertyManager->addProperty(QVariant::Color, QLatin1String(" Color Property"));
    topItem->addSubProperty(item);

    mPropertyEditor->addProperty(veryTopItem);
    mPropertyEditor->addProperty(topItem);

    mGui->layout->addWidget(mPropertyEditor);
    mGui->layout->addWidget(Core::newLineWidget(this));
















    // Create our Simulation collapsible widget

    mSimulationWidget = new SingleCellSimulationViewInformationSimulationWidget(this);
    mSimulationCollapsibleWidget = new Core::CollapsibleWidget(QString(), mSimulationWidget, this);

    mSimulationWidget->setObjectName("Simulation");
    mSimulationCollapsibleWidget->setObjectName("SimulationCollapsible");

    // Create our Solvers collapsible widget

    mSolversWidget = new SingleCellSimulationViewInformationSolversWidget(this);
    mSolversCollapsibleWidget = new Core::CollapsibleWidget(QString(), mSolversWidget, this);

    mSolversWidget->setObjectName("Solvers");
    mSolversCollapsibleWidget->setObjectName("SolversCollapsible");

    // Create our Traces collapsible widget

    mTracesCollapsibleWidget = new Core::CollapsibleWidget(this);

    mTracesCollapsibleWidget->setObjectName("TracesCollapsible");

    // Create our Parameters collapsible widget

    mParametersWidget = new SingleCellSimulationViewInformationParametersWidget(this);
    mParametersCollapsibleWidget = new Core::CollapsibleWidget(QString(), mParametersWidget, this);

    mParametersWidget->setObjectName("Parameters");
    mParametersCollapsibleWidget->setObjectName("ParametersCollapsible");

    // Add our collapsible widgets to our layout
    // Note: we add a stretch at the end to make sure that our collapsible
    //       widgets take as little vertical space as possible...

    mGui->layout->addWidget(mSimulationCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mSolversCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mTracesCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mParametersCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addStretch(1);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Make our simulation collapsible widget our focus proxy

    setFocusProxy(mSimulationCollapsibleWidget);
}

//==============================================================================

SingleCellSimulationViewInformationWidget::~SingleCellSimulationViewInformationWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);

    // Retranslate our sections

    mSimulationSection->setPropertyName(tr("Simulation"));
    mSolversSection->setPropertyName(tr("Solvers"));
    mTracesSection->setPropertyName(tr("Traces"));
    mParametersSection->setPropertyName(tr("Parameters"));

    // Retranslate the title of our collapsible widgets

    mSimulationCollapsibleWidget->setTitle(tr("Simulation"));
    mSolversCollapsibleWidget->setTitle(tr("Solvers"));
    mTracesCollapsibleWidget->setTitle(tr("Traces"));
    mParametersCollapsibleWidget->setTitle(tr("Parameters"));

    // Retranslate our various collapsible widgets' body

    mSimulationWidget->retranslateUi();
    mSolversWidget->retranslateUi();
    mParametersWidget->retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our simulation collapsible widget and its body

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our solvers collapsible widget and its body

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our traces collapsible widget and its body

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our parameters collapsible widget and its body

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our simulation collapsible widget and its body

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our solvers collapsible widget and its body

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our traces collapsible widget and its body

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our parameters collapsible widget and its body

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget * SingleCellSimulationViewInformationWidget::simulationWidget()
{
    // Return our simulation widget

    return mSimulationWidget;
}

//==============================================================================

SingleCellSimulationViewInformationSolversWidget * SingleCellSimulationViewInformationWidget::solversWidget()
{
    // Return our solvers widget

    return mSolversWidget;
}

//==============================================================================

SingleCellSimulationViewInformationParametersWidget * SingleCellSimulationViewInformationWidget::parametersWidget()
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
