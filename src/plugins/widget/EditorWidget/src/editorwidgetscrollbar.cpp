/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Editor scroll bar
//==============================================================================

#include "editorwidgeteditorwidget.h"
#include "editorwidgetscrollbar.h"

//==============================================================================

#include <QPainter>
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    #include <QStyleOptionSlider>
#endif

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorWidgetScrollBar::EditorWidgetScrollBar(EditorWidgetEditorWidget *pParent) :
    QScrollBar(pParent),
    mOwner(pParent)
{
}

//==============================================================================

void EditorWidgetScrollBar::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QScrollBar::paintEvent(pEvent);

    // Retrieve the height of our arrow buttons on Windows and Linux
    // Note: I was hoping to use styleOption.initFrom(this) and thus have a
    //       truly cross-platform solution, but although it rightly returns 0 on
    //       macOS, it returns the height of the whole scroll bar on Windows and
    //       Linux...

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    QStyleOptionSlider styleOption;

    initStyleOption(&styleOption);

    int arrowButtonHeight = style()->subControlRect(QStyle::CC_ScrollBar, &styleOption, QStyle::SC_ScrollBarAddLine, this).height();
#elif defined(Q_OS_MAC)
    int arrowButtonHeight = 0;
#else
    #error Unsupported platform
#endif

    // Draw our position

    static const QPen PositionPen = QPen(Qt::darkGray);

    int line;
    int lastLine;
    int dummy;

    mOwner->getCursorPosition(&line, &dummy);
    mOwner->lineIndexFromPosition(mOwner->text().length(), &lastLine, &dummy);

    double positionScaling = double(height()-2*arrowButtonHeight-1)/lastLine;
    int cursorPosition = arrowButtonHeight+line*positionScaling;

    QPainter painter(this);

    painter.setPen(PositionPen);
    painter.drawLine(0, cursorPosition, width(), cursorPosition);

    // Draw our highlights

    static const QPen HighlightPen = QColor(0, 192, 0);

    painter.setPen(HighlightPen);

    foreach (int highlightedLine, mOwner->highlightedLines()) {
        cursorPosition = arrowButtonHeight+highlightedLine*positionScaling;

        painter.drawLine(0, cursorPosition, width(), cursorPosition);
    }
}

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
