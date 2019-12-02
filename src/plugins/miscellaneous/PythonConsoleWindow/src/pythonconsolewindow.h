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
// Python Console window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QWidget>

//==============================================================================

namespace Ui {
    class PythonConsoleWindow;
} // namespace Ui

//==============================================================================

namespace OpenCOR {
namespace PythonConsoleWindow {

//==============================================================================

class PythonConsoleWindow : public Core::WindowWidget
{
Q_OBJECT

public:
    explicit PythonConsoleWindow(QWidget *pParent);
    ~PythonConsoleWindow() override;

    void retranslateUi() override;

private:
    Ui::PythonConsoleWindow *mGui;

    QWidget *mPythonConsoleWidget;
};

//==============================================================================

} // namespace PythonConsoleWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
