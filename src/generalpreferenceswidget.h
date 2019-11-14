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
// General preferences widget
//==============================================================================

#pragma once

//==============================================================================

#include "preferencesinterface.h"

//==============================================================================

namespace Ui {
    class GeneralPreferencesWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsPreferencesStyle = QStringLiteral("Style");

//==============================================================================

#if defined(Q_OS_WIN)
    static const auto SettingsPreferencesStyleDefault = QStringLiteral("windowsvista");
#elif defined(Q_OS_LINUX)
    static const auto SettingsPreferencesStyleDefault = QStringLiteral("Fusion");
#else
    static const auto SettingsPreferencesStyleDefault = QStringLiteral("macintosh");
#endif

//==============================================================================

class GeneralPreferencesWidget : public Preferences::PreferencesWidget
{
    Q_OBJECT

public:
    explicit GeneralPreferencesWidget(QWidget *pParent);
    ~GeneralPreferencesWidget() override;

    bool preferencesChanged() const override;

    void resetPreferences() override;
    void savePreferences() override;

private:
    Ui::GeneralPreferencesWidget *mGui;

    QString mStyle;
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
