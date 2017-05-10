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
// Graph Panel widget custom axes dialog
//==============================================================================

#include "coreguiutils.h"
#include "graphpanelwidgetcustomaxesdialog.h"
#include "plugin.h"

//==============================================================================

#include "ui_graphpanelwidgetcustomaxesdialog.h"

//==============================================================================

#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelWidgetCustomAxesDialog::GraphPanelWidgetCustomAxesDialog(const double &pMinX,
                                                                   const double &pMaxX,
                                                                   const double &pMinY,
                                                                   const double &pMaxY,
                                                                   QWidget *pParent) :
    Core::Dialog(pParent),
    mGui(new Ui::GraphPanelWidgetCustomAxesDialog)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    // Create our 'special' settings
    // Note: special in the sense that we don't retrieve them from the plugin
    //       itself since this is not a view, a window or anything like that...

    mSettings = new QSettings();

    mSettings->beginGroup(SettingsPlugins);
    mSettings->beginGroup("GraphPanelWidget");
    mSettings->beginGroup("GraphPanelWidgetCustomAxesDialog");

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

GraphPanelWidgetCustomAxesDialog::~GraphPanelWidgetCustomAxesDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

double GraphPanelWidgetCustomAxesDialog::minX() const
{
    // Return our minimum X value

    return mGui->xMinValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesDialog::maxX() const
{
    // Return our maximum X value

    return mGui->xMaxValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesDialog::minY() const
{
    // Return our minimum Y value

    return mGui->yMinValue->text().toDouble();
}

//==============================================================================

double GraphPanelWidgetCustomAxesDialog::maxY() const
{
    // Return our maximum Y value

    return mGui->yMaxValue->text().toDouble();
}

//==============================================================================

void GraphPanelWidgetCustomAxesDialog::checkValue(QLineEdit *pValue)
{
    // Check that we don't have an empty value and if we do then set the value
    // to zero and select it

    if (pValue->text().isEmpty()) {
        pValue->setText("0");
        pValue->selectAll();
    }
}

//==============================================================================


void GraphPanelWidgetCustomAxesDialog::on_xMinValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our X-min value

    checkValue(mGui->xMinValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesDialog::on_xMaxValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our X-max value

    checkValue(mGui->xMaxValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesDialog::on_yMinValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our Y-min value

    checkValue(mGui->yMinValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesDialog::on_yMaxValue_textEdited(const QString &pValue)
{
    Q_UNUSED(pValue);

    // Check our Y-max value

    checkValue(mGui->yMaxValue);
}

//==============================================================================

void GraphPanelWidgetCustomAxesDialog::on_buttonBox_accepted()
{
    // Check that the values make sense

    bool xProblem = minX() >= maxX();
    bool yProblem = minY() >= maxY();

    if (xProblem && yProblem) {
        Core::warningMessageBox(tr("Custom Axes"),
                                tr("X-min and Y-min must be lower than X-max and Y-max, respectively."));
    } else if (xProblem) {
        Core::warningMessageBox(tr("Custom Axes"),
                                tr("X-min must be lower than X-max."));
    } else if (yProblem) {
        Core::warningMessageBox(tr("Custom Axes"),
                                tr("Y-min must be lower than Y-max."));
    } else {
        // Confirm that we accepted the changes

        accept();
    }
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
