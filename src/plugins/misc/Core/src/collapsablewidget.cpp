//==============================================================================
// Collapsable widget
//==============================================================================

#include "collapsablewidget.h"
#include "coreutils.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void CollapsableWidget::constructor(const QString &pTitle, QWidget *pBody)
{
    // Some initialisations

    mBody = pBody;

    // Create a vertical layout which will contain our header and body

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // Create our header

    QWidget *header = new QWidget(this);

    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    headerLayout->setMargin(0);
    headerLayout->setSpacing(0);

    mTitle = new QLabel(pTitle, header);
    mButton = new QToolButton(header);

#ifdef Q_WS_MAC
    mTitle->setAlignment(Qt::AlignCenter);
#endif

    int iconSize = 0.4*mTitle->height();

    mButton->setIconSize(QSize(iconSize, iconSize));

    connect(mButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsableState()));

#ifdef Q_WS_MAC
    headerLayout->addWidget(mButton);
#endif
    headerLayout->addWidget(mTitle);
#ifndef Q_WS_MAC
    headerLayout->addWidget(mButton);
#endif

    header->setLayout(headerLayout);

    // Create our separator

    mSeparator = Core::newLineWidget(this);

    // Update our GUI by showing our widget collapsed or not, depdending on
    // whether there is a body

    updateGui(!pBody);

    // Populate our main layout

    mainLayout->addWidget(header);
    mainLayout->addWidget(mSeparator);

    if (pBody)
        mainLayout->addWidget(pBody);
    else
        mSeparator->hide();

    // Apply the main layout to ourselves

    setLayout(mainLayout);
}

//==============================================================================

CollapsableWidget::CollapsableWidget(const QString &pTitle,
                                     QWidget *pBody, QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent)
{
    // Construct our object

    constructor(pTitle, pBody);
}

//==============================================================================

CollapsableWidget::CollapsableWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

QSize CollapsableWidget::sizeHint() const
{
    // Suggest a default size for our collapsable widget

    return defaultSize(0);
}

//==============================================================================

QString CollapsableWidget::title() const
{
    // Return our title

    return mTitle->text();
}

//==============================================================================

void CollapsableWidget::setTitle(const QString &pTitle)
{
    // Set our title

    if (pTitle.compare(mTitle->text()))
        mTitle->setText(pTitle);
}

//==============================================================================

QWidget * CollapsableWidget::body() const
{
    // Return our body

    return mBody;
}

//==============================================================================

void CollapsableWidget::setBody(QWidget *pBody)
{
    // Set our body

    if (pBody != mBody) {
        bool bodyBefore = mBody;

        if (mBody)
            layout()->removeWidget(mBody);

        mBody = pBody;

        if (pBody) {
            layout()->addWidget(pBody);

            // Update our GUI, using the previous collapsable state of our
            // widget, in case there was already a body before, or by asking
            // it to be uncollapsed now that there is a body

            updateGui(bodyBefore?mCollapsed:false);
        } else {
            // There is no body (still or anymore), so collapse ourselves

            updateGui(true);
        }
    }
}

//==============================================================================

void CollapsableWidget::setCollapsed(const bool &pCollapsed)
{
    // Collapse or uncollapse ourselves, if needed

    if (pCollapsed != mCollapsed)
        updateGui(pCollapsed);
}

//==============================================================================

bool CollapsableWidget::isCollapsed() const
{
    // Return wheter we are collapsed

    return mCollapsed;
}

//==============================================================================

void CollapsableWidget::updateGui(const bool &pCollapsed)
{
    // Update our widget's GUI

    mCollapsed = pCollapsed;

    if (pCollapsed) {
#ifdef Q_WS_MAC
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-right.png"));
#else
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-left.png"));
#endif
    }
    else {
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));
    }

    mSeparator->setVisible(!pCollapsed);

    if (mBody)
        mBody->setVisible(!pCollapsed);

    mButton->setEnabled(mBody);
}

//==============================================================================

void CollapsableWidget::toggleCollapsableState()
{
    // Toggle the collapsable state of our widget and update its GUI

    updateGui(!mCollapsed);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
