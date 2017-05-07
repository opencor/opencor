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
// View widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "widget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT ViewWidget : public Widget
{
    Q_OBJECT

public:
    explicit ViewWidget(QWidget *pParent);

    virtual QWidget * widget(const QString &pFileName) = 0;

    virtual QList<QWidget *> statusBarWidgets() const;

signals:
    void updateFileTabIcon(const QString &pViewName, const QString &pFileName,
                           const QIcon &pIcon);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
