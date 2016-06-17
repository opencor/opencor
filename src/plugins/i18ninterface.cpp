/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Internationalisation interface
//==============================================================================

#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QMenu>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void I18nInterface::setLocale(const QString &pLocale)
{
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mPluginName, pLocale));
    qApp->installTranslator(&mTranslator);
}

//==============================================================================

void I18nInterface::setPluginName(const QString &pPluginName)
{
    // Set the name of the plugin

    mPluginName = pPluginName;
}

//==============================================================================

void I18nInterface::retranslateMenu(QMenu *pMenu, const QString &pTitle)
{
    // Retranslate the menu, i.e. retranslate its title

    pMenu->setTitle(pTitle);
}

//==============================================================================

void I18nInterface::retranslateAction(QAction *pAction,
                                      const QString &pTextAndToolTip,
                                      const QString &pStatusTip)
{
    // Retranslate the action, i.e. retranslate its text, tool tip and status
    // tip

    pAction->setText(pTextAndToolTip);
    pAction->setToolTip(pTextAndToolTip);
    pAction->setStatusTip(pStatusTip);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
