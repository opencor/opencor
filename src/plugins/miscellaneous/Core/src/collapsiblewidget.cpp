/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Collapsible widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreguiutils.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QSettings>
#include <QToolButton>
#include <QVariant>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CollapsibleHeaderTitleWidget::CollapsibleHeaderTitleWidget(QWidget *pParent) :
    QLabel(pParent)
{
}

//==============================================================================

void CollapsibleHeaderTitleWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QLabel::mouseDoubleClickEvent(pEvent);

    // Let people know that the widget has been double clicked

    emit doubleClicked();
}

//==============================================================================

CollapsibleHeaderWidget::CollapsibleHeaderWidget(const QColor &pSeparatorColor,
                                                 const bool &pCollapsible,
                                                 QWidget *pParent) :
    QWidget(pParent),
    mCollapsed(false),
    mLastHeader(false)
{
    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    // Create a sub-widget with a horizontal layout

    QWidget *subWidget = new QWidget(this);
    QHBoxLayout *subLayout = new QHBoxLayout(subWidget);

    subLayout->setMargin(0);
    subLayout->setSpacing(0);

    subWidget->setLayout(subLayout);

    QColor winColor = windowColor();

    setStyleSheet(QString("QWidget {"
                          "    background: rgb(%1, %2, %3);"
                          "}").arg(QString::number(winColor.red()),
                                   QString::number(winColor.green()),
                                   QString::number(winColor.blue())));

    // Create and customise our button and title

    static const QIcon NoIcon   = QIcon();
    static const QIcon DownIcon = QIcon(":/oxygen/actions/arrow-down.png");

    mButton = new QToolButton(subWidget);
    mTitle  = new CollapsibleHeaderTitleWidget(subWidget);

    int iconSize = 0.4*mTitle->height();

    mButton->setIcon(pCollapsible?DownIcon:NoIcon);
    mButton->setIconSize(QSize(iconSize, iconSize));
    mButton->setStyleSheet("QToolButton {"
                           "    border: 0px;"
                           "}"
                           ""
                           "QToolButton:pressed {"
                           "    margin: 0px;"
                           "}");

    mTitle->setAlignment(Qt::AlignCenter);

    // Add our button and title to our sub-layout

    subLayout->addWidget(mButton);
    subLayout->addWidget(mTitle);

    // Add a top separator, our sub-widget and a bottom separator to our main
    // layout
    // Note: we keep track of our top and bottom separators since we may need
    //       to hide them in some cases/circumstances...

    mTopSeparator    = newLineWidget(pSeparatorColor, this);
    mBottomSeparator = newLineWidget(pSeparatorColor, this);

    layout->addWidget(mTopSeparator);
    layout->addWidget(subWidget);
    layout->addWidget(mBottomSeparator);

    // Connections to toggle our collapsed state

    if (pCollapsible) {
        connect(mButton, SIGNAL(clicked(bool)),
                this, SLOT(toggleCollapsedState()));
        connect(mTitle, SIGNAL(doubleClicked()),
                this, SLOT(toggleCollapsedState()));
    }
}

//==============================================================================

void CollapsibleHeaderWidget::setFirstHeader(const bool &pFirstHeader)
{
    // Show/hide our top separator depending on whether we are the first header

    mTopSeparator->setVisible(!pFirstHeader);
}

//==============================================================================

void CollapsibleHeaderWidget::setLastHeader(const bool &pLastHeader)
{
    // Keep track of our last header status and update our bottom separator
    // visible status, if needed

    if (pLastHeader != mLastHeader) {
        mLastHeader = pLastHeader;

        updateBottomSeparatorVisibleStatus();
    }
}

//==============================================================================

bool CollapsibleHeaderWidget::isCollapsable() const
{
    // Return wheter we are collapsable

    return !mButton->icon().isNull();
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

void CollapsibleHeaderWidget::updateBottomSeparatorVisibleStatus()
{
    // Show/hide our bottom separator depending on whether we are collapsed or
    // whether we are the last header

    mBottomSeparator->setVisible(!mCollapsed || mLastHeader);
}

//==============================================================================

void CollapsibleHeaderWidget::toggleCollapsedState()
{
    // Toggle our collapsed state

    mCollapsed = !mCollapsed;

    // Update our button's icon to reflect our new collapsed state

    static const QIcon DownIcon  = QIcon(":/oxygen/actions/arrow-down.png");
    static const QIcon RightIcon = QIcon(":/oxygen/actions/arrow-right.png");

    mButton->setIcon(mCollapsed?RightIcon:DownIcon);

    // Update our bottom separator visible status

    updateBottomSeparatorVisibleStatus();

    // Let people know about our new new collapsed state

    emit widgetVisible(!mCollapsed);
}

//==============================================================================

void CollapsibleWidget::constructor(const QColor &pSeparatorColor)
{
    // Some initialisations

    mSeparatorColor = pSeparatorColor;
    mHeaders = QList<CollapsibleHeaderWidget *>();

    // Create a vertical layout that will contain our headers and widgets

    mLayout = new QVBoxLayout(this);

    mLayout->setMargin(0);
    mLayout->setSpacing(0);

    setLayout(mLayout);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(const QColor &pSeparatorColor,
                                     QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor(pSeparatorColor);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

int CollapsibleWidget::count() const
{
    // Return our number of headers

    return mHeaders.count();
}

//==============================================================================

bool CollapsibleWidget::isCollapsed(const int &pIndex) const
{
    // Return wheter our requested header is collapsed

    return mHeaders.value(pIndex)?mHeaders[pIndex]->isCollapsed():false;
}

//==============================================================================

void CollapsibleWidget::setCollapsed(const int &pIndex, const bool &pCollapsed)
{
    // Collapse or uncollapse our requested header

    if (mHeaders.value(pIndex))
        mHeaders[pIndex]->setCollapsed(pCollapsed);
}

//==============================================================================

QString CollapsibleWidget::headerTitle(const int &pIndex) const
{
    // Return the title of our requested header

    return mHeaders.value(pIndex)?mHeaders[pIndex]->title():QString();
}

//==============================================================================

void CollapsibleWidget::setHeaderTitle(const int &pIndex, const QString &pTitle)
{
    // Set the title of our requested header

    if (mHeaders.value(pIndex))
        mHeaders[pIndex]->setTitle(pTitle);
}

//==============================================================================

void CollapsibleWidget::addWidget(QWidget *pWidget, const bool &pCollapsible)
{
    // Make sure that there is a widget to add

    if (!pWidget)
        return;

    // We want to add a widget, so we first need to add a header to our layout

    CollapsibleHeaderWidget *header = new CollapsibleHeaderWidget(mSeparatorColor, pCollapsible, this);

    // Let our header know whether it is the first header

    header->setFirstHeader(mHeaders.isEmpty());

    // Let our header know that, if anything, it is our new last header, meaning
    // that our previous last header is not the last one anymore

    if (!mHeaders.isEmpty())
        mHeaders.last()->setLastHeader(false);

    header->setLastHeader(true);

    // Now, we can actually add the header to our layout, as well as keep track
    // of it

    mLayout->addWidget(header);

    mHeaders << header;

    // Add the widget itself

    mLayout->addWidget(pWidget);

    // Create a connection to show/hide our widget depending on the collapsed
    // state of our header, as well as one to let people know about our overall
    // new collapsible state

    if (pCollapsible) {
        connect(header, SIGNAL(widgetVisible(const bool &)),
                pWidget, SLOT(setVisible(bool)));

        connect(header, SIGNAL(widgetVisible(const bool &)),
                this, SLOT(emitCollapsed()));
    }
}

//==============================================================================

void CollapsibleWidget::emitCollapsed()
{
    // Let people know that a collapsed state has changed

    for (int i = 0, iMax = mHeaders.count(); i < iMax; ++i) {
        if (mHeaders[i] == sender()) {
            emit collapsed(i, mHeaders[i]->isCollapsed());

            return;
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
