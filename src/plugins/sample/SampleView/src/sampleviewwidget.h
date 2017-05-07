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
// Sample view widget
//==============================================================================

#pragma once

//==============================================================================

#include "viewwidget.h"

//==============================================================================

namespace Ui {
    class SampleViewWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

class SampleViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SampleViewWidget(QWidget *pParent);
    ~SampleViewWidget();

    virtual void retranslateUi();

    virtual QWidget * widget(const QString &pFileName);

    void update(const QString &pFileName);

private:
    Ui::SampleViewWidget *mGui;

    QString mFileName;
};

//==============================================================================

}   // namespace SampleView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
