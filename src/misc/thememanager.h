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
// Theme manager
//==============================================================================

#pragma once

//==============================================================================

#include <QPalette>

//==============================================================================

class QApplication;

//==============================================================================

namespace OpenCOR {

//==============================================================================

enum class Theme
{
    Light,
    Dark
};

//==============================================================================

class ThemeManager
{
public:
    static Theme currentTheme();
    static void setTheme(Theme pTheme);
    static void applyTheme(QApplication *pApplication);

    static QPalette darkPalette(const QPalette &pLightPalette);

private:
    static void applyTheme(QApplication *pApplication, Theme pTheme);
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
