//==============================================================================
// Bordered widget
//==============================================================================

#include "borderedwidget.h"
#include "coreutils.h"

//==============================================================================

#include <QFrame>
#include <QGridLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

BorderedWidget::BorderedWidget(QWidget *pWidget,
                               const bool &pTop, const bool &pLeft,
                               const bool &pBottom, const bool &pRight) :
    Widget(pWidget->parentWidget())
{
    // We want our widget to be bordered by a single line which colour matches
    // that of the application's theme. Because of that colour requirement, we
    // can't use a QFrame, so instead we rely on the use of a grid layout and
    // Core::newLineWidget()...

    // Create a grid layout for ourselves

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    setLayout(gridLayout);

    // Add some real line widgets to the top, left, bottom and/or right of
    // ourselves, if required

    int fromColumn = pLeft?0:1;
    int columnSpan = pLeft+1+pRight;

    if (pTop)
        gridLayout->addWidget(Core::newLineWidget(this, true),
                              0, fromColumn, 1, columnSpan);

    if (pLeft)
        gridLayout->addWidget(Core::newLineWidget(this, false), 1, 0);

    gridLayout->addWidget(pWidget, 1, 1);

    if (pBottom)
        gridLayout->addWidget(Core::newLineWidget(this, true),
                              2, fromColumn, 1, columnSpan);

    if (pRight)
        gridLayout->addWidget(Core::newLineWidget(this, false), 1, 2);

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
