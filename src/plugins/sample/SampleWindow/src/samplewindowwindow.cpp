/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Sample window
//==============================================================================

#include "sampleutils.h"
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
