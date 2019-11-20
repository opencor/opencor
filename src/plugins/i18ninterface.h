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

#pragma once

//==============================================================================

#include <QTranslator>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int i18nInterfaceVersion();

//==============================================================================

class I18nInterface
{
public:
    virtual ~I18nInterface();

    void updateTranslator(const QString &pTranslator);

#define INTERFACE_DEFINITION
    #include "i18ninterface.inl"
#undef INTERFACE_DEFINITION

    static void retranslateMenu(QMenu *pMenu, const QString &pTitle);
    static void retranslateAction(QAction *pAction,
                                  const QString &pTextAndToolTip,
                                  const QString &pStatusTip);

private:
    QTranslator mTranslator;
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::I18nInterface, "OpenCOR::I18nInterface")

//==============================================================================
// End of file
//==============================================================================
