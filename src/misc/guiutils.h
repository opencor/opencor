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
// GUI utilities
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QColor>
#include <QMessageBox>
#include <QSettings>
#include <QStyledItemDelegate>

//==============================================================================

class QAction;
class QMainWindow;
class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "guiutils.h.inl"

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define
//       Dialog. To have it in guiutils.h.inl is NOT good enough since the MOC
//       won't pick it up...

class CORE_EXPORT Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *pParent);

    bool hasPositionAndSize();

protected:
    QSettings mSettings;

    void resizeEvent(QResizeEvent *pEvent) override;

public slots:
    int exec() override;
};

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define
//       StyledItemDelegate. To have it in guiutils.h.inl is NOT good enough
//       since the MOC won't pick it up...

class CORE_EXPORT StyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StyledItemDelegate(QObject *pParent);

    QSize sizeHint(const QStyleOptionViewItem &pOption,
                   const QModelIndex &pIndex) const override;
};

//==============================================================================

bool CORE_EXPORT aboutToQuit();

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
