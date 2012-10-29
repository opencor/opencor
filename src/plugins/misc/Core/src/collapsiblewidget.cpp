//==============================================================================
// Collapsible widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void CollapsibleWidget::constructor(const QString &pTitle, QWidget *pBody)
{
    // Some initialisations

    mBody = pBody;

    mFirstHeightUpdate = true;
    mOldHeight = 0;

    // Create a vertical layout which will contain our header and body

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    // Create our header

    mHeader = new QWidget(this);

    QHBoxLayout *headerLayout = new QHBoxLayout(mHeader);

    headerLayout->setMargin(0);
    headerLayout->setSpacing(0);

    mTitle = new QLabel(pTitle, mHeader);
    mButton = new QToolButton(mHeader);

#ifdef Q_WS_MAC
    mTitle->setAlignment(Qt::AlignCenter);
#endif

    int iconSize = 0.4*mTitle->height();

    mButton->setIconSize(QSize(iconSize, iconSize));

    connect(mButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsibleState()));

#ifdef Q_WS_MAC
    headerLayout->addWidget(mButton);
#endif
    headerLayout->addWidget(mTitle);
#ifndef Q_WS_MAC
    headerLayout->addWidget(mButton);
#endif

    mHeader->setLayout(headerLayout);

    // Create our separator

    mSeparator = Core::newLineWidget(this);

    // Create our scroll area widget
    // Note: the idea is to have our the body of our collapsible widget within
    //       a scroll area, this in case the body is too wide...

    mScrollArea = new QScrollArea(this);

    mScrollArea->setAlignment(Qt::AlignCenter);
    mScrollArea->setFrameStyle(QFrame::NoFrame);

    // Populate our main layout

    mainLayout->addWidget(mHeader);
    mainLayout->addWidget(mSeparator);
    mainLayout->addWidget(mScrollArea);

    if (pBody)
        mScrollArea->setWidget(pBody);

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
        bool bodyBefore = mBody;

        mBody = pBody;

        if (pBody) {
            mScrollArea->setWidget(pBody);

            // Update our GUI, using the previous collapsible state of our
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

void CollapsibleWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Widget::resizeEvent(pEvent);

    // Resize our height, if needed

    if (!mCollapsed && mBody) {
        // Determine what the height of our scroll area widget should be

        int newScrollAreaHeight = mBody->height();

        if (mScrollArea->width() < mBody->width())
            // We need to account for the horizontal scroll bar

            newScrollAreaHeight += mScrollArea->horizontalScrollBar()->height();

        // Update our height and that of our scroll area widget
        // Note: our height must be updated after that of our scroll area
        //       widget since our initial height will be determined by that of
        //       the scroll area (hence, also, we don't update our height the
        //       very first time round)...

        int scrollAreaHeightDiff = newScrollAreaHeight-mScrollArea->height();

        mScrollArea->setFixedHeight(newScrollAreaHeight);

        if (mFirstHeightUpdate)
            mFirstHeightUpdate = false;
        else
            setFixedHeight(height()+scrollAreaHeightDiff);
    }
}

//==============================================================================

void CollapsibleWidget::updateGui(const bool &pCollapsed)
{
    // Update our widget's GUI

    mCollapsed = pCollapsed;

    //  Customise some widgets

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

    // Enable/disable some widgets

    mTitle->setEnabled(mBody);
    mButton->setEnabled(mBody);

    // Show/hide some widgets

    mSeparator->setVisible(!pCollapsed);
    mScrollArea->setVisible(!pCollapsed);

    if (!mFirstHeightUpdate) {
        if (pCollapsed) {
            mOldHeight = height();

            setFixedHeight(mHeader->height());
        } else if (mOldHeight) {
            setFixedHeight(mOldHeight);//mTitle->height()+mSeparator->height()+mBody->height()+20);
        }
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
