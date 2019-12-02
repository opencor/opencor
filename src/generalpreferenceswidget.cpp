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
// General preferences widget
//==============================================================================

#include "generalpreferenceswidget.h"

//==============================================================================

#include "ui_generalpreferenceswidget.h"

//==============================================================================

#include <QApplication>
#include <QSettings>
#include <QStyleFactory>

//==============================================================================

namespace OpenCOR {

//==============================================================================

GeneralPreferencesWidget::GeneralPreferencesWidget(QWidget *pParent) :
    Preferences::PreferencesWidget(Preferences::GeneralPreferences, pParent),
    mGui(new Ui::GeneralPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    QStringList styles = QStyleFactory::keys();

    styles.sort(Qt::CaseInsensitive);

    mGui->styleValue->addItems(styles);

    mStyle = mSettings.value(SettingsPreferencesStyle, SettingsPreferencesStyleDefault).toString();

    mGui->styleValue->setCurrentText(mStyle);

    setFocusProxy(mGui->styleValue);
}

//==============================================================================

GeneralPreferencesWidget::~GeneralPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

bool GeneralPreferencesWidget::preferencesChanged() const
{
    // Return whether our preferences have changed

    return mGui->styleValue->currentText() != mStyle;
}

//==============================================================================

void GeneralPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    mGui->styleValue->setCurrentText(SettingsPreferencesStyleDefault);
}

//==============================================================================

void GeneralPreferencesWidget::savePreferences()
{
    // Save our preferences

    QString style = mGui->styleValue->currentText();

    mSettings.setValue(SettingsPreferencesStyle, style);

    // Update our style

    QApplication::setStyle(style);
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
