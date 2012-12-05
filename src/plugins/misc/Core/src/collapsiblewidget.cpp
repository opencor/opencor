//==============================================================================
// Collapsible widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QSettings>
#include <QToolButton>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void CollapsibleWidget::constructor(const QString &pTitle, QWidget *pBody)
{
    // Some initialisations

    mBody = pBody;

    // Create a vertical layout which will contain our header and body

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // Create our header

    mHeader = new QWidget(this);

    QHBoxLayout *headerLayout = new QHBoxLayout(mHeader);

    headerLayout->setMargin(0);
    headerLayout->setSpacing(0);

    mButton = new QToolButton(mHeader);
    mTitle  = new QLabel(QString(), mHeader);

    int iconSize = 0.4*mTitle->height();

    mButton->setIconSize(QSize(iconSize, iconSize));
    mButton->setStyleSheet("QToolButton {"
                           "    border: 0px;"
                           "}"
                           ""
                           "QToolButton::pressed {"
                           "    margin: 0px;"
                           "}");

    connect(mButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsibleState()));

    mTitle->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(mButton);
    headerLayout->addWidget(mTitle);

    mHeader->setLayout(headerLayout);

    // Create our separator

    mSeparator = Core::newLineWidget(this);

    // Populate our main layout

    mainLayout->addWidget(mHeader);
    mainLayout->addWidget(mSeparator);

    if (pBody)
        mainLayout->addWidget(pBody);

    // Apply the main layout to ourselves

    setLayout(mainLayout);

    // Update our GUI by showing our widget collapsed or not, depdending on
    // whether there is a body

    updateGui(!pBody);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(const QString &pTitle,
                                     QWidget *pBody, QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor(pTitle, pBody);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

static const QString SettingsCollapsed = "Collapsed";

//==============================================================================

void CollapsibleWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our collapsable state

    setCollapsed(pSettings->value(SettingsCollapsed, false).toBool());
}

//==============================================================================

void CollapsibleWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our collapsable state

    pSettings->setValue(SettingsCollapsed, isCollapsed());
}

//==============================================================================

QString CollapsibleWidget::title() const
{
    // Return our title

    return mTitle->text();
}

//==============================================================================

void CollapsibleWidget::setTitle(const QString &pTitle)
{
    // Set our title

    if (pTitle.compare(mTitle->text()))
        mTitle->setText(pTitle);
}

//==============================================================================

QWidget * CollapsibleWidget::body() const
{
    // Return our body

    return mBody;
}

//==============================================================================

void CollapsibleWidget::setBody(QWidget *pBody)
{
    // Set our body

    if (pBody != mBody) {
        // Remove the old body, if any

        if (mBody)
            layout()->removeWidget(mBody);

        // Add the new body, if any

        if (pBody)
            layout()->addWidget(pBody);

        // Determine what our collapsed state should be

        bool collapsed = pBody?
                             mBody?mCollapsed:false:
                             true;

        // Keep track of the new body

        mBody = pBody;

        // Update our GUI

        updateGui(collapsed);
    }
}

//==============================================================================

void CollapsibleWidget::setCollapsed(const bool &pCollapsed)
{
    // Collapse or uncollapse ourselves, if needed

    if (pCollapsed != mCollapsed)
        updateGui(pCollapsed);
}

//==============================================================================

bool CollapsibleWidget::isCollapsed() const
{
    // Return wheter we are collapsed

    return mCollapsed;
}

//==============================================================================

void CollapsibleWidget::updateGui(const bool &pCollapsed)
{
    // Update our widget's GUI

    mCollapsed = pCollapsed && mBody;

    //  Customise our button

    if (pCollapsed)
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-right.png"));
    else
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));

    // Enable/disable some widgets

    mTitle->setEnabled(mBody);
    mButton->setEnabled(mBody);

    // Show/hide our separator

    mSeparator->setVisible(!pCollapsed);

    // Show/hide our body

    if (mBody)
        mBody->setVisible(!pCollapsed);

    // Update our focus proxy and make sure that the new focus proxy immediately
    // gets the focus
    // Note: if we were not to immediately give the new focus proxy the focus,
    //       then the central widget would give the focus to the previously
    //       focused widget (see CentralWidget::updateGui()), so...

    if (pCollapsed || !mBody) {
        setFocusProxy(0);

        setFocus();
    } else {
        setFocusProxy(mBody);

        mBody->setFocus();
    }
}

//==============================================================================

void CollapsibleWidget::toggleCollapsibleState()
{
    // Toggle the collapsible state of our widget and update its GUI

    updateGui(!mCollapsed);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
