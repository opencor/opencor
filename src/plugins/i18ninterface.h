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

#pragma once

//==============================================================================

#include <QString>
#include <QTranslator>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class I18nInterface
{
public:
    void setLocale(const QString &pLocale);
    void setPluginName(const QString &pPluginName);

#define INTERFACE_DEFINITION
    #include "i18ninterface.inl"
#undef INTERFACE_DEFINITION

    static void retranslateMenu(QMenu *pMenu, const QString &pTitle);
    static void retranslateAction(QAction *pAction, const QString &pText,
                                  const QString &pStatusTip);

private:
    QString mPluginName;

    QTranslator mTranslator;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::I18nInterface, "OpenCOR::I18nInterface")

//==============================================================================
// End of file
//==============================================================================
