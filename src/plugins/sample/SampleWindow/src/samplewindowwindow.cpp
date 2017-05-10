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
// Sample window
//==============================================================================

#include "sampleutilities.h"
#include "samplewindowwindow.h"

//==============================================================================

#include "ui_samplewindowwindow.h"

//==============================================================================

namespace OpenCOR {
namespace SampleWindow {

//==============================================================================

SampleWindowWindow::SampleWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::SampleWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // A couple of connections to update our sum whenever one of the value of
    // one of our numbers is updated

    connect(mGui->nb1DoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateSum()));
    connect(mGui->nb2DoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateSum()));

    // Initialise our sum

    updateSum();
}

//==============================================================================

SampleWindowWindow::~SampleWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SampleWindowWindow::updateSum()
{
    // Update our sum

    mGui->sumLabel->setText(QString::number(Sample::add(mGui->nb1DoubleSpinBox->value(), mGui->nb2DoubleSpinBox->value())));
}

//==============================================================================

}   // namespace SampleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
