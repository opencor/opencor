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
// Core GUI utilities
//==============================================================================

void updateColors()
{
    // Retrieve the colour used for a 'normal' border
    // Note #1: we use a QStackedWidget object and retrieve the colour of the
    //          pixel which is in the middle of the right border...
    // Note #2: we don't rely on the top border because it may be rendered in a
    //          special way. Also, we don't rely on a corner as such in case
    //          it's rendered as a rounded corner...
    // Note #3: on OS X, our widget must be shown otherwise, the retrieved
    //          border colour will be black. So we show it off screen, even on
    //          Windows and Linux, in case their behaviour was to change in the
    //          future...

    // Create our widget and show it off screen

    QStackedWidget stackedWidget;

    stackedWidget.setFrameShape(QFrame::StyledPanel);

    stackedWidget.move(-2*stackedWidget.width(), -2*stackedWidget.height());
    stackedWidget.show();

    // Render the widget to an image

    QImage image = QImage(stackedWidget.size(),
                          QImage::Format_ARGB32_Premultiplied);

    stackedWidget.render(&image);

    // Retrieve the colour we are after

    QColor borderColor = QColor(image.pixel(image.width()-1, 0.5*image.height()));

    // Use our settings to keep track of the colour

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBorderColor, borderColor);
    settings.endGroup();

    // Retrieve some other colours

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBaseColor, qApp->palette().color(QPalette::Base));
        settings.setValue(SettingsHighlightColor, qApp->palette().color(QPalette::Highlight));
        settings.setValue(SettingsLinkColor, qApp->palette().color(QPalette::Link));
        settings.setValue(SettingsShadowColor, qApp->palette().color(QPalette::Shadow));
        settings.setValue(SettingsWindowColor, qApp->palette().color(QPalette::Window));
    settings.endGroup();
}

//==============================================================================

void showEnableAction(QAction *pAction, const bool &pVisible,
                      const bool &pEnabled)
{
    // Show/enable or hide/disable the given action

    pAction->setVisible(pVisible);
    pAction->setEnabled(pVisible && pEnabled);
}

//==============================================================================

QColor specificColor(const QString &pColor)
{
    // Return a specific colour

    QColor res;
    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        res = settings.value(pColor).value<QColor>();
    settings.endGroup();

    return res;
}

//==============================================================================

QColor baseColor()
{
    // Return the base colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsBaseColor);
}

//==============================================================================

QColor borderColor()
{
    // Return the border colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::retrieveBorderColor())...

    return specificColor(SettingsBorderColor);
}

//==============================================================================

QColor highlightColor()
{
    // Return the highlight colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsHighlightColor);
}

//==============================================================================

QColor linkColor()
{
    // Return the link colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsLinkColor);
}

//==============================================================================

QColor shadowColor()
{
    // Return the shadow colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsShadowColor);
}

//==============================================================================

QColor windowColor()
{
    // Return the window colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsWindowColor);
}

//==============================================================================
// End of file
//==============================================================================
