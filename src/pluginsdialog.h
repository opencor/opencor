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
// Plugins dialog
//==============================================================================

#pragma once

//==============================================================================

#include "guiutils.h"
#include "plugininfo.h"

//==============================================================================

class QSettings;

//==============================================================================

namespace Ui {
    class PluginsDialog;
} // namespace Ui

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginItemDelegate : public StyledItemDelegate
{
public:
    explicit PluginItemDelegate(QObject *pParent);

    void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
               const QModelIndex &pIndex) const override;
};

//==============================================================================

class Plugin;
class PluginItem;
class PluginItemModel;
class PluginManager;

//==============================================================================

class PluginsDialog : public Dialog
{
    Q_OBJECT

public:
    explicit PluginsDialog(PluginManager *pPluginManager, QWidget *pParent);
    ~PluginsDialog() override;

private:
    Ui::PluginsDialog *mGui;

    PluginManager *mPluginManager;

    PluginItemModel *mModel;

    QList<PluginItem *> mSelectablePluginItems;
    QList<PluginItem *> mUnselectablePluginItems;

    QMap<QString, bool> mInitialLoadingStates;

    QMap<PluginInfo::Category, PluginItem *> mCategoryItems;
    QMap<PluginItem *, PluginInfo::Category> mItemCategories;

    PluginItem * pluginCategoryItem(PluginInfo::Category pCategory);

    QString statusDescription(Plugin *pPlugin) const;

    void updatePluginsSelectedState(PluginItem *pItem, bool pInitializing);

private slots:
    void treeViewCollapsed(const QModelIndex &pIndex);

    void selectablePluginsCheckBoxToggled(bool pChecked);

    void buttonBoxAccepted();

    void updateInformation(const QModelIndex &pNewIndex,
                           const QModelIndex &pOldIndex);
    void updatePluginsSelectedState(PluginItem *pItem);

    void openLink(const QString &pLink) const;

    void apply();
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
