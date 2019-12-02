/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Graph Panel widget custom axes dialog
//==============================================================================

#pragma once

//==============================================================================

#include "coreguiutils.h"

//==============================================================================

namespace Ui {
    class GraphPanelWidgetCustomAxesDialog;
} // namespace Ui

//==============================================================================

class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

class GraphPanelWidgetCustomAxesDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit GraphPanelWidgetCustomAxesDialog(double pMinX, double pMaxX,
                                              double pMinY, double pMaxY,
                                              QWidget *pParent);
    ~GraphPanelWidgetCustomAxesDialog() override;

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

private:
    Ui::GraphPanelWidgetCustomAxesDialog *mGui;

    void checkValue(QLineEdit *pValue);

private slots:
    void xMinValueTextEdited(const QString &pValue);
    void xMaxValueTextEdited(const QString &pValue);
    void yMinValueTextEdited(const QString &pValue);
    void yMaxValueTextEdited(const QString &pValue);

    void buttonBoxAccepted();
};

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
