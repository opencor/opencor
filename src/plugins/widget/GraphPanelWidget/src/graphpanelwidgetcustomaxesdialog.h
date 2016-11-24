/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Graph Panel widget custom axes dialog
//==============================================================================

#pragma once

//==============================================================================

#include <QDialog>

//==============================================================================

namespace Ui {
    class GraphPanelWidgetCustomAxesDialog;
}

//==============================================================================

class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

class GraphPanelWidgetCustomAxesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphPanelWidgetCustomAxesDialog(const double &pMinX,
                                              const double &pMaxX,
                                              const double &pMinY,
                                              const double &pMaxY,
                                              QWidget *pParent);
    ~GraphPanelWidgetCustomAxesDialog();

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

private:
    Ui::GraphPanelWidgetCustomAxesDialog *mGui;

    void checkValue(QLineEdit *pValue);

private slots:
    void on_xMinValue_textEdited(const QString &pValue);
    void on_xMaxValue_textEdited(const QString &pValue);
    void on_yMinValue_textEdited(const QString &pValue);
    void on_yMaxValue_textEdited(const QString &pValue);

    void on_buttonBox_accepted();
};

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
