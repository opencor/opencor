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
// Common widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "plugin.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QColor>
#include <QSettings>
#include <QString>

//==============================================================================

class QSize;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class BusyWidget;

//==============================================================================

class CORE_EXPORT CommonWidget
{
public:
    explicit CommonWidget(QWidget *pParent);
    virtual ~CommonWidget();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings) const;

    virtual void retranslateUi();

    bool isBusyWidgetVisible() const;

    void processEvents();

protected:
    QSize defaultSize(double pRatio) const;

private:
    QWidget *mParent;

    BusyWidget *mBusyWidget = nullptr;

    int mCounter = 0;

    void showBusyWidget(double pProgress);

public slots:
    void showBusyWidget();
    void showProgressBusyWidget();
    void setBusyWidgetProgress(double pProgress);
    void hideBusyWidget(bool pForceHiding = false);
    void resizeBusyWidget();
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
