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

#include "thememanager.h"

//==============================================================================

#include <QApplication>
#include <QSettings>
#include <QStyle>
#include <QWidget>
#include <QtMath>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsPreferences = QStringLiteral("Preferences");
static const auto SettingsAppearance  = QStringLiteral("Appearance");
static const auto SettingsTheme       = QStringLiteral("Theme");

static const auto LightTheme = QStringLiteral("Light");
static const auto DarkTheme  = QStringLiteral("Dark");

static const auto ThemeProperty = "OpenCOR::Theme";

//==============================================================================

static QPalette sLightPalette;
static bool sLightPaletteSet = false;

//==============================================================================

Theme ThemeManager::currentTheme()
{
    if ((qApp != nullptr) && qApp->property(ThemeProperty).isValid()) {
        return qApp->property(ThemeProperty).toString() == DarkTheme?
                   Theme::Dark:
                   Theme::Light;
    }

    QSettings settings;

    settings.beginGroup(SettingsPreferences);
    settings.beginGroup(SettingsAppearance);

    return settings.value(SettingsTheme, DarkTheme).toString() == DarkTheme?
               Theme::Dark:
               Theme::Light;
}

//==============================================================================

void ThemeManager::setTheme(Theme pTheme)
{
    QSettings settings;

    settings.beginGroup(SettingsPreferences);
    settings.beginGroup(SettingsAppearance);
    settings.setValue(SettingsTheme, pTheme == Theme::Dark? DarkTheme:LightTheme);

    if (qApp != nullptr) {
        applyTheme(qApp, pTheme);
    }
}

//==============================================================================

void ThemeManager::applyTheme(QApplication *pApplication)
{
    if (pApplication == nullptr) {
        return;
    }

    if (!sLightPaletteSet) {
        sLightPalette = pApplication->palette();
        sLightPaletteSet = true;
    }

    applyTheme(pApplication, currentTheme());
}

//==============================================================================

static qreal luminance(const QColor &pColor)
{
    auto component = [](qreal pValue) {
        return pValue <= 0.03928?
                   pValue/12.92:
                   qPow((pValue+0.055)/1.055, 2.4);
    };

    return   0.2126*component(pColor.redF())
           + 0.7152*component(pColor.greenF())
           + 0.0722*component(pColor.blueF());
}

//==============================================================================

static QColor visibleAccent(const QColor &pColor)
{
    QColor res = pColor;

    while (luminance(res) < 0.18) {
        res = QColor::fromRgbF(0.8*res.redF()+0.2,
                              0.8*res.greenF()+0.2,
                              0.8*res.blueF()+0.2);
    }

    return res;
}

//==============================================================================

QPalette ThemeManager::darkPalette(const QPalette &pLightPalette)
{
    QPalette res = pLightPalette;

    const QColor black = QColor(0x00, 0x00, 0x00);
    const QColor white = QColor(0xff, 0xff, 0xff);
    const QColor button = QColor(0x20, 0x20, 0x20);
    const QColor alternateBase = QColor(0x18, 0x18, 0x18);
    const QColor disabledText = QColor(0x80, 0x80, 0x80);
    const QColor placeholderText = QColor(0xa0, 0xa0, 0xa0);
    const QColor toolTipBase = QColor(0x28, 0x28, 0x28);
    const QColor accent = visibleAccent(pLightPalette.color(QPalette::Active,
                                                            QPalette::Highlight));
    const QColor highlightedText = luminance(accent) > 0.45? black:white;
    const QColor link = visibleAccent(pLightPalette.color(QPalette::Active,
                                                          QPalette::Link));
    const QColor visitedLink = visibleAccent(pLightPalette.color(QPalette::Active,
                                                                 QPalette::LinkVisited));

    for (auto group : { QPalette::Active, QPalette::Inactive }) {
        res.setColor(group, QPalette::Window, black);
        res.setColor(group, QPalette::WindowText, white);
        res.setColor(group, QPalette::Base, black);
        res.setColor(group, QPalette::AlternateBase, alternateBase);
        res.setColor(group, QPalette::Text, white);
        res.setColor(group, QPalette::Button, button);
        res.setColor(group, QPalette::ButtonText, white);
        res.setColor(group, QPalette::ToolTipBase, toolTipBase);
        res.setColor(group, QPalette::ToolTipText, white);
        res.setColor(group, QPalette::BrightText, white);
        res.setColor(group, QPalette::Highlight, accent);
        res.setColor(group, QPalette::HighlightedText, highlightedText);
        res.setColor(group, QPalette::Link, link);
        res.setColor(group, QPalette::LinkVisited, visitedLink);
        res.setColor(group, QPalette::Light, QColor(0x45, 0x45, 0x45));
        res.setColor(group, QPalette::Midlight, QColor(0x35, 0x35, 0x35));
        res.setColor(group, QPalette::Mid, QColor(0x30, 0x30, 0x30));
        res.setColor(group, QPalette::Dark, QColor(0x08, 0x08, 0x08));
        res.setColor(group, QPalette::Shadow, black);
        res.setColor(group, QPalette::PlaceholderText, placeholderText);
    }

    res.setColor(QPalette::Disabled, QPalette::Window, black);
    res.setColor(QPalette::Disabled, QPalette::WindowText, disabledText);
    res.setColor(QPalette::Disabled, QPalette::Base, black);
    res.setColor(QPalette::Disabled, QPalette::AlternateBase, alternateBase);
    res.setColor(QPalette::Disabled, QPalette::Text, disabledText);
    res.setColor(QPalette::Disabled, QPalette::Button, QColor(0x18, 0x18, 0x18));
    res.setColor(QPalette::Disabled, QPalette::ButtonText, disabledText);
    res.setColor(QPalette::Disabled, QPalette::ToolTipBase, toolTipBase);
    res.setColor(QPalette::Disabled, QPalette::ToolTipText, disabledText);
    res.setColor(QPalette::Disabled, QPalette::BrightText, white);
    res.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0x38, 0x38, 0x38));
    res.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledText);
    res.setColor(QPalette::Disabled, QPalette::Link, disabledText);
    res.setColor(QPalette::Disabled, QPalette::LinkVisited, disabledText);
    res.setColor(QPalette::Disabled, QPalette::PlaceholderText, disabledText);

    return res;
}

//==============================================================================

void ThemeManager::applyTheme(QApplication *pApplication, Theme pTheme)
{
    if (!sLightPaletteSet) {
        sLightPalette = pApplication->palette();
        sLightPaletteSet = true;
    }

    pApplication->setProperty(ThemeProperty,
                              pTheme == Theme::Dark? DarkTheme:LightTheme);
    pApplication->setPalette(pTheme == Theme::Dark?
                                 darkPalette(sLightPalette):
                                 sLightPalette);

    // Ensure that widgets with style-polished palette caches refresh too.

    for (auto widget : pApplication->allWidgets()) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
