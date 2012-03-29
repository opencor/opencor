//==============================================================================
// Bordered widget
//==============================================================================

#include "borderedwidget.h"
#include "coreutils.h"

//==============================================================================

#include <QFrame>
#include <QBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

BorderedWidget::BorderedWidget(QWidget *pWidget,
                               const Location &pLocation) :
    Core::Widget(qobject_cast<QWidget *>(pWidget->parent()))
{
    // Create a layout for ourselves

    QBoxLayout *layout;

    if ((pLocation == Left) || (pLocation == Right))
        layout = new QHBoxLayout(this);
    else
        layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setLayout(layout);

    // Populate our vertical layout with a real line and our bordered widget

    if ((pLocation == Left) || (pLocation == Top))
        layout->addWidget(Core::newRealLineWidget(this));

    layout->addWidget(pWidget);

    if ((pLocation == Right) || (pLocation == Bottom))
        layout->addWidget(Core::newRealLineWidget(this));

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
