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
// Single cell view information simulation widget
//==============================================================================

#pragma once

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewInformationSimulationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationSimulationWidget(QWidget *pParent);

    virtual void retranslateUi();

    void initialize(SingleCellViewSimulation *pSimulation);

    Core::Property * startingPointProperty() const;
    Core::Property * endingPointProperty() const;
    Core::Property * pointIntervalProperty() const;

    double startingPoint() const;
    double endingPoint() const;
    double pointInterval() const;

private:
    Core::Property *mStartingPointProperty;
    Core::Property *mEndingPointProperty;
    Core::Property *mPointIntervalProperty;

    void updateToolTips();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
