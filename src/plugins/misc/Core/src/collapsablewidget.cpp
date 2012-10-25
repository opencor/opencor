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
//    mainLayout->setSpacing(0);

    // Create our header

    QWidget *header = new QWidget(this);

    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    headerLayout->setMargin(0);

    mTitle = new QLabel(pTitle, header);
    mButton = new QToolButton(header);

#ifdef Q_WS_MAC
    mTitle->setAlignment(Qt::AlignCenter);
#endif

    if (pBody) {
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));
    }
    else {
#ifdef Q_WS_MAC
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-right.png"));
#else
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-left.png"));
#endif

        mButton->setEnabled(false);
    }

    int iconSize = 0.4*mTitle->height();

    mButton->setIconSize(QSize(iconSize, iconSize));

//    mButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    mButton->set

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
    Widget(pParent)
{
    // Construct our object

    constructor(pTitle, pBody);
}

//==============================================================================

CollapsableWidget::CollapsableWidget(const QString &pTitle,
                                     QWidget *pParent) :
    Widget(pParent)
{
    // Construct our object

    constructor(pTitle);
}

//==============================================================================

CollapsableWidget::CollapsableWidget(QWidget *pParent) :
    Widget(pParent)
{
    // Construct our object

    constructor();
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
        if (mBody) {
            mSeparator->hide();

            layout()->removeWidget(mBody);
        }

        mBody = pBody;

        if (pBody) {
            mSeparator->show();

            layout()->addWidget(pBody);
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
