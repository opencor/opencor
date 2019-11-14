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
#else
    int arrowButtonHeight = 0;
#endif

    // Draw our highlights

    static int PenAlpha = 96;
    static const QPen HighlightPen = QColor(0, 192, 0, PenAlpha);

    QPainter painter(this);
    double positionScaling = (height()-2*arrowButtonHeight-1)/double(mOwner->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, mOwner->text().length()));
    int cursorPosition;
    QIntSet cursorPositions = QIntSet();

    painter.setPen(HighlightPen);

    for (auto highlightedLine : mOwner->highlightedLines()) {
        cursorPosition = int(arrowButtonHeight+highlightedLine*positionScaling);

        if (!cursorPositions.contains(cursorPosition)) {
            cursorPositions << cursorPosition;

            painter.drawLine(0, cursorPosition, width(), cursorPosition);
        }
    }

    // Draw our position

    static const QPen PositionPen = QColor(0, 0, 0, PenAlpha);

    int line = int(mOwner->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, mOwner->SendScintilla(QsciScintilla::SCI_GETCURRENTPOS)));

    cursorPosition = int(arrowButtonHeight+line*positionScaling);

    painter.setPen(PositionPen);
    painter.drawLine(0, cursorPosition, width(), cursorPosition);
}

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
