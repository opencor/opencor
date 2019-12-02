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
// Tab bar widget Mac style
//==============================================================================

#pragma once

//==============================================================================

#include <QProxyStyle>
#include <QStyleOptionTab>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class TabBarWidgetMacStyle : public QProxyStyle
{
    Q_OBJECT

public:
    void drawControl(ControlElement pElement, const QStyleOption *pOption,
                     QPainter *pPainter, const QWidget *pWidget = nullptr) const override;

    QRect subElementRect(SubElement pElement, const QStyleOption *pOption,
                         const QWidget *pWidget) const override;

private:
    void tabLayout(const QStyleOptionTab *pOption, const QWidget *pWidget,
                   QRect *pTextRect, QRect *pIconRect) const;
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
