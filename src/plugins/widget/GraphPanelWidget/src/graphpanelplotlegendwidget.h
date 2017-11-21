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

#pragma once

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_abstract_legend.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

class GraphPanelPlotLegendWidget : public QwtAbstractLegend
{
    Q_OBJECT

public:
    explicit GraphPanelPlotLegendWidget(QWidget *pParent);

    virtual void renderLegend(QPainter *pPainter, const QRectF &pRect,
                              bool pFillBackground) const;

    virtual bool isEmpty() const;

public slots:
    virtual void updateLegend(const QVariant &pItemInfo,
                              const QList<QwtLegendData> &pData);
};

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
