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

CollapsibleHeaderWidget::CollapsibleHeaderWidget(QWidget *pParent) :
    QWidget(pParent),
    mCollapsed(false)
{
    // Create a horizontal layout and add a button and label to it

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setMargin(0);
    layout->setSpacing(0);

    mButton = new QToolButton(this);
    mTitle  = new QLabel(QString(), this);

    int iconSize = 0.4*mTitle->height();

    mButton->setIconSize(QSize(iconSize, iconSize));
    mButton->setStyleSheet("QToolButton {"
                           "    border: 0px;"
                           "}"
                           ""
                           "QToolButton::pressed {"
                           "    margin: 0px;"
                           "}");

    mTitle->setAlignment(Qt::AlignCenter);

    layout->addWidget(mButton);
    layout->addWidget(mTitle);

    setLayout(layout);

    // Create a connection to keep track of the collapsed state of our header

    connect(mButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsedState()));
}

//==============================================================================

bool CollapsibleHeaderWidget::isCollapsed() const
{
    // Return wheter we are collapsed

    return mCollapsed;
}

//==============================================================================

void CollapsibleHeaderWidget::setCollapsed(const bool &pCollapsed)
{
    // Collapse or uncollapse ourselves, if needed

    if (pCollapsed != mCollapsed)
        toggleCollapsedState();
}

//==============================================================================

QString CollapsibleHeaderWidget::title() const
{
    // Return our title

    return mTitle->text();
}

//==============================================================================

void CollapsibleHeaderWidget::setTitle(const QString &pTitle)
{
    // Set our title

    mTitle->setText(pTitle);
}

//==============================================================================

void CollapsibleHeaderWidget::toggleCollapsedState()
{
    // Toggle our collapsed state

    mCollapsed = !mCollapsed;

    // Update our button's icon to reflect our new collapsed state

    if (mCollapsed)
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-right.png"));
    else
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));

    // Let people know about our new new collapsed state

    emit contentsVisible(!mCollapsed);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(QWidget *pParent) :
    Widget(QSize(), pParent),
    mHeaders(QList<CollapsibleHeaderWidget *>()),
    mBodies(QList<QWidget *>())
{
    // Create a vertical layout which will contain our headers and widgets

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // Apply the main layout to ourselves

    setLayout(mainLayout);
}

//==============================================================================

static const QString SettingsCollapsed = "Collapsed%1";

//==============================================================================

void CollapsibleWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our collapsable state

    for (int i = 0, iMax = mHeaders.count(); i < iMax; ++i)
        setCollapsed(i, pSettings->value(SettingsCollapsed.arg(i), false).toBool());
}

//==============================================================================

void CollapsibleWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our collapsable state

    for (int i = 0, iMax = mHeaders.count(); i < iMax; ++i)
        pSettings->setValue(SettingsCollapsed.arg(i), isCollapsed(i));
}

//==============================================================================

bool CollapsibleWidget::isCollapsed(const int &pIndex) const
{
    // Return wheter our requested header is collapsed

    return mHeaders.value(pIndex)?mHeaders.at(pIndex)->isCollapsed():false;
}

//==============================================================================

void CollapsibleWidget::setCollapsed(const int &pIndex, const bool &pCollapsed)
{
    // Collapse or uncollapse our requested header

    if (mHeaders.value(pIndex))
        mHeaders.at(pIndex)->setCollapsed(pCollapsed);
}

//==============================================================================

QString CollapsibleWidget::headerTitle(const int &pIndex) const
{
    // Return the title of our requested header

    return mHeaders.value(pIndex)?mHeaders.at(pIndex)->title():QString();
}

//==============================================================================

void CollapsibleWidget::setHeaderTitle(const int &pIndex, const QString &pTitle)
{
    // Set the title of our requested header

    if (mHeaders.value(pIndex))
        mHeaders.at(pIndex)->setTitle(pTitle);
}

//==============================================================================

void CollapsibleWidget::addWidget(QWidget *pWidget)
{
    // We want to add a new widget, so we first need to add a new header to our
    // layout

    CollapsibleHeaderWidget *header = new CollapsibleHeaderWidget(this);

    layout()->addWidget(header);

    mHeaders.append(header);

    // Now, we can add the new widget itself

    layout()->addWidget(pWidget);

    mBodies.append(pWidget);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
