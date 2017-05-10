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
// CellML Annotation view widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;
class CellMLAnnotationViewPlugin;

//==============================================================================

class CellmlAnnotationViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(CellMLAnnotationViewPlugin *pPlugin,
                                        QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    void filePermissionsChanged(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    virtual QWidget * widget(const QString &pFileName);

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    static QString resourceUrl(const QString &pResource);
    static QString idUrl(const QString &pResource, const QString &pId);

private:
    CellMLAnnotationViewPlugin *mPlugin;

    CellmlAnnotationViewEditingWidget *mEditingWidget;
    QMap<QString, CellmlAnnotationViewEditingWidget *> mEditingWidgets;

    QIntList mEditingWidgetSizes;
    QIntList mMetadataDetailsWidgetSizes;

private slots:
    void editingWidgetSplitterMoved(const QIntList &pSizes);
    void metadataDetailsWidgetSplitterMoved(const QIntList &pSizes);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
