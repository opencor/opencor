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
// Plugins dialog
//==============================================================================

#pragma once

//==============================================================================

#include "guiutils.h"
#include "coreinterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QStyledItemDelegate>

//==============================================================================

class QSettings;
class QStandardItem;
class QStandardItemModel;

//==============================================================================

namespace Ui {
    class PluginsDialog;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginManager;

//==============================================================================

class PluginItemDelegate : public QStyledItemDelegate
{
public:
    explicit PluginItemDelegate(QObject *pParent);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class PluginsDialog : public Dialog
{
    Q_OBJECT

public:
    explicit PluginsDialog(QSettings *pSettings, PluginManager *pPluginManager,
                           QWidget *pParent);
    ~PluginsDialog();

private:
    Ui::PluginsDialog *mGui;

    PluginManager *mPluginManager;

    QStandardItemModel *mModel;

    QList<QStandardItem *> mSelectablePluginItems;
    QList<QStandardItem *> mUnselectablePluginItems;

    QMap<QString, bool> mInitialLoadingStates;

    QMap<PluginInfo::Category, QStandardItem *> mCategoryItems;
    QMap<QStandardItem *, PluginInfo::Category> mItemCategories;

    QStandardItem * pluginCategoryItem(const PluginInfo::Category &pCategory);

    QString statusDescription(Plugin *pPlugin) const;

private slots:
    void on_treeView_collapsed(const QModelIndex &pIndex);

    void on_selectablePluginsCheckBox_toggled(bool pChecked);

    void on_buttonBox_accepted();

    void updateInformation(const QModelIndex &pNewIndex,
                           const QModelIndex &pOldIndex);
    void updatePluginsSelectedState(QStandardItem *pItem,
                                    const bool &pInitializing = false);

    void openLink(const QString &pLink) const;

    void apply();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
