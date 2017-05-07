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
// Internationalisation interface
//==============================================================================

#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QMenu>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int i18nInterfaceVersion()
{
    // Version of the internationalisation interface

    return 1;
}

//==============================================================================

void I18nInterface::updateTranslator(const QString &pTranslator)
{
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);

    mTranslator.load(pTranslator);

    qApp->installTranslator(&mTranslator);
}

//==============================================================================

void I18nInterface::retranslateMenu(QMenu *pMenu, const QString &pTitle)
{
    // Retranslate our menu, i.e. retranslate its title

    pMenu->setTitle(pTitle);
}

//==============================================================================

void I18nInterface::retranslateAction(QAction *pAction,
                                      const QString &pTextAndToolTip,
                                      const QString &pStatusTip)
{
    // Retranslate our action, i.e. retranslate its text, tool tip and status
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
