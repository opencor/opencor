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
// Graph panel plot legend widget
//==============================================================================

#include "graphpanelplotlegendwidget.h"

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_painter.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelPlotLegendWidget::GraphPanelPlotLegendWidget(QWidget *pParent) :
    QwtAbstractLegend(pParent)
{
//---ISSUE1171--- TO BE DONE......
}

//==============================================================================

void GraphPanelPlotLegendWidget::renderLegend(QPainter *pPainter,
                                              const QRectF &pRect,
                                              bool pFillBackground) const
{
//---ISSUE1171--- TO BE DONE......
    // Draw our background, if required

    if (   pFillBackground
        && (autoFillBackground() || testAttribute( Qt::WA_StyledBackground))) {
        QwtPainter::drawBackgound(pPainter, pRect, this);
    }
}

//==============================================================================

bool GraphPanelPlotLegendWidget::isEmpty() const
{
//---ISSUE1171--- TO BE DONE......
    return false;
}

//==============================================================================

void GraphPanelPlotLegendWidget::updateLegend(const QVariant &pItemInfo,
                                              const QList<QwtLegendData> &pData)
{
//---ISSUE1171--- TO BE DONE......
    Q_UNUSED(pItemInfo);
    Q_UNUSED(pData);
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
