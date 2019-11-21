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
// Preferences interface
//==============================================================================

#pragma once

//==============================================================================

#include <QSettings>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Preferences {

//==============================================================================

static const auto SettingsPreferences = QStringLiteral("Preferences");

//==============================================================================

static const auto GeneralPreferences = QStringLiteral("General");

//==============================================================================

class PreferencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(const QString &pName, QWidget *pParent);

    virtual bool preferencesChanged() const = 0;

    virtual void resetPreferences() = 0;
    virtual void savePreferences() = 0;

protected:
    QSettings mSettings;
};

//==============================================================================

} // namespace Preferences

//==============================================================================

extern "C" Q_DECL_EXPORT int preferencesInterfaceVersion();

//==============================================================================

class PreferencesInterface
{
public:
    virtual ~PreferencesInterface();

#define INTERFACE_DEFINITION
    #include "preferencesinterface.inl"
#undef INTERFACE_DEFINITION

    static QVariant preference(const QString &pName, const QString &pKey,
                               const QVariant &pDefaultValue = QVariant());
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::PreferencesInterface, "OpenCOR::PreferencesInterface")

//==============================================================================
// End of file
//==============================================================================
