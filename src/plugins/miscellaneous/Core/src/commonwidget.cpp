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
// Common widget
//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QPainter>
#include <QPen>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is a ratio of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(),
                 pRatio*desktopGeometry.height());
}

//==============================================================================

void CommonWidget::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
