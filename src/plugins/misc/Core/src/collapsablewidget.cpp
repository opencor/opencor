//==============================================================================
// Collapsable widget
//==============================================================================

#include "collapsablewidget.h"

//==============================================================================

#include <QLabel>
#include <QLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CollapsableWidget::CollapsableWidget(const QString &pTitle,
                                     QWidget *pBody, QWidget *pParent) :
    Widget(pParent),
    mBody(pBody)
{
    // Create a vertical layout which will contain our header and body

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setMargin(0);
//    mainLayout->setSpacing(0);

    // Create our header

    QWidget *header = new QWidget(this);

    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    headerLayout->setMargin(0);

    mTitle = new QLabel(pTitle, header);

    headerLayout->addWidget(mTitle);

    header->setLayout(headerLayout);

    // Populate our main layout

    mainLayout->addWidget(header);
    mainLayout->addWidget(mBody);

    // Apply the main layout to ourselves

    setLayout(mainLayout);
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
        layout()->removeWidget(mBody);

        mBody = pBody;

        layout()->addWidget(pBody);
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
