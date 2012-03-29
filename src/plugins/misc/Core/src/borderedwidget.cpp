//==============================================================================
// Bordered widget
//==============================================================================

#include "borderedwidget.h"
#include "coreutils.h"

//==============================================================================

#include <QFrame>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

BorderedWidget::BorderedWidget(QWidget *pWidget) :
    Core::Widget(qobject_cast<QWidget *>(pWidget->parent()))
{
    // Create a layout for ourselves

    QVBoxLayout *verticalLayout= new QVBoxLayout(this);

    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setSpacing(0);

    setLayout(verticalLayout);

    // Populate our vertical layout with a real line and our bordered widget

    verticalLayout->addWidget(Core::newRealLineWidget(this));
    verticalLayout->addWidget(pWidget);

    // Keep track of our bordered widget

    mWidget = pWidget;
}

//==============================================================================

QWidget * BorderedWidget::widget()
{
    // Return our bordered widget

    return mWidget;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
