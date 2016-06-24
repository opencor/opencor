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
// Graph Panel widget custom axes window
//==============================================================================

#include "graphpanelwidgetcustomaxeswindow.h"

//==============================================================================

#include <QMessageBox>
#include <QRegularExpressionValidator>

//==============================================================================

#include "ui_graphpanelwidgetcustomaxeswindow.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelWidgetCustomAxesWindow::GraphPanelWidgetCustomAxesWindow(const double &pMinX,
                                                                   const double &pMaxX,
                                                                   const double &pMinY,
                                                                   const double &pMaxY,
                                                                   QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::GraphPanelWidgetCustomAxesWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Only allow double numbers

    QRegularExpressionValidator *doubleRegExValidator = new QRegularExpressionValidator(QRegularExpression("^[+-]?(\\d+(\\.\\d*)?|\\.\\d+)([eE][+-]?\\d+)?$"), this);

    mGui->xMinValue->setValidator(doubleRegExValidator);
    mGui->xMaxValue->setValidator(doubleRegExValidator);
    mGui->yMinValue->setValidator(doubleRegExValidator);
    mGui->yMaxValue->setValidator(doubleRegExValidator);

    // Populate ourselves

    mGui->xMinValue->setText(QString::number(pMinX));
    mGui->xMaxValue->setText(QString::number(pMaxX));
    mGui->yMinValue->setText(QString::number(pMinY));
    mGui->yMaxValue->setText(QString::number(pMaxY));

    // Select all of the minimum X value

    mGui->xMinValue->selectAll();
}

//==============================================================================

GraphPanelWidgetCustomAxesWindow::~GraphPanelWidgetCustomAxesWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

double GraphPanelWidgetCustomAxesWindow::minX() const
{
    // Return our minimum X value

    return mGui->xMinValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesWindow::maxX() const
{
    // Return our maximum X value

    return mGui->xMaxValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesWindow::minY() const
{
    // Return our minimum Y value

    return mGui->yMinValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesWindow::maxY() const
{
    // Return our maximum Y value

    return mGui->yMaxValue->text().toDouble();
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::checkValue(QLineEdit *pValue)
{
    // Check that we don't have an empty value and if we do then set the value
    // to zero and select it

    if (pValue->text().isEmpty()) {
        pValue->setText("0");
        pValue->selectAll();
    }
}

//==============================================================================


void GraphPanelWidgetCustomAxesWindow::on_xMinValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our X-min value

    checkValue(mGui->xMinValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::on_xMaxValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our X-max value

    checkValue(mGui->xMaxValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::on_yMinValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our Y-min value

    checkValue(mGui->yMinValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::on_yMaxValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our Y-max value

    checkValue(mGui->yMaxValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::on_buttonBox_accepted()
{
    // Check that the values make sense

    bool xProblem = minX() >= maxX();
    bool yProblem = minY() >= maxY();

    if (xProblem && yProblem) {
        QMessageBox::warning(this, tr("Custom Axes"),
                             tr("X-min and Y-min must be lower than X-max and Y-max, respectively."));
    } else if (xProblem) {
        QMessageBox::warning(this, tr("Custom Axes"),
                             tr("X-min must be lower than X-max."));
    } else if (yProblem) {
        QMessageBox::warning(this, tr("Custom Axes"),
                             tr("Y-min must be lower than Y-max."));
    } else {
        // Confirm that we accepted the changes

        accept();
    }
}

//==============================================================================

void GraphPanelWidgetCustomAxesWindow::on_buttonBox_rejected()
{
    // Simply cancel whatever was done here

    reject();
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
