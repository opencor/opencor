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
// GUI utilities
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QColor>
#include <QMessageBox>

//==============================================================================

class QAction;
class QMainWindow;

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "coreguiutils.h.inl"

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define MessageBox.
//       To have it in guiutils.h.inl is NOT good enough since the MOC won't
//       pick it up...

class CORE_EXPORT MessageBox : public QMessageBox
{
    Q_OBJECT

public:
    static void about(QWidget *pParent, const Qt::TextInteractionFlags &pFlags,
                      const QString &pTitle, const QString &pText);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
