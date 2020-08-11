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
// Mapping view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "cellmlzincmappingvieweditingwidget.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace ZincWidget {
    class ZincWidget;
}   // namespace ZincWidget

//==============================================================================

namespace CellMLZincMappingView {

//==============================================================================

class CellMLZincMappingViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit CellMLZincMappingViewWidget(QWidget *pParent);
    ~CellMLZincMappingViewWidget() override;

    void retranslateUi() override;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    CellMLZincMappingViewEditingWidget * editingWidget(const QString &pFileName) const;

    QWidget * widget(const QString &pFileName) override;

    void filePermissionsChanged(const QString &pFileName);
    void fileSaved(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

protected:
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;

private:

    CellMLZincMappingViewEditingWidget* mEditingWidget = nullptr;
    QMap<QString, CellMLZincMappingViewEditingWidget*> mEditingWidgets;

    QString mMeshFileName;

    QMap<QString, FileTypeInterface *> mFileTypeInterfaces;

};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
