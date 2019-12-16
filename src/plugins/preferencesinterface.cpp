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
// Preferences interface
//==============================================================================

#include "plugin.h"
#include "preferencesinterface.h"

//==============================================================================

#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int preferencesInterfaceVersion()
{
    // Version of the preferences interface

    return 2;
}

//==============================================================================

namespace Preferences {

//==============================================================================

PreferencesWidget::PreferencesWidget(const QString &pName, QWidget *pParent) :
    QWidget(pParent)
{
    // Customise our settings

    if (pName != GeneralPreferences) {
        mSettings.beginGroup(SettingsPlugins);
        mSettings.beginGroup(pName);
    }

    mSettings.beginGroup(SettingsPreferences);
}

//==============================================================================

} // namespace Preferences

//==============================================================================

PreferencesInterface::~PreferencesInterface() = default;

//==============================================================================

QVariant PreferencesInterface::preference(const QString &pName,
                                          const QString &pKey,
                                          const QVariant &pDefaultValue)
{
    // Retrieve the preference, which key is given, associated with the given
    // set of preferences, which name is given, and for which we have a given
    // default value in case the preference doesn't exist

    QSettings settings;

    if (pName != Preferences::GeneralPreferences) {
        settings.beginGroup(SettingsPlugins);
        settings.beginGroup(pName);
    }

    settings.beginGroup(Preferences::SettingsPreferences);

    return settings.value(pKey, pDefaultValue);
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
