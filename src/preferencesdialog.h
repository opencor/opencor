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
// Preferences dialog
//==============================================================================

#pragma once

//==============================================================================

#include "guiutils.h"
#include "plugininfo.h"

//==============================================================================

#include <QStyledItemDelegate>

//==============================================================================

class QLabel;
class QSettings;
class QStandardItem;
class QStandardItemModel;

//==============================================================================

namespace Ui {
    class PreferencesDialog;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginManager;
class PreferencesInterface;

//==============================================================================

namespace Preferences {
    class PreferencesWidget;
}   // namespace Preferences

//==============================================================================

class PreferencesPluginCategoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesPluginCategoryWidget(QWidget *pParent);

    void setCategory(const QString &pCategory);
    void setDescription(const QString &pDescription);

private:
    QLabel *mCategoryValue;
    QLabel *mDescriptionValue;

    QLabel * label(const QString &pLabel);
};

//==============================================================================

class PreferencesItemDelegate : public QStyledItemDelegate
{
public:
    explicit PreferencesItemDelegate(QObject *pParent);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class PreferencesDialog : public Dialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QSettings *pSettings,
                               PluginManager *pPluginManager,
                               const QString &pPluginName, QWidget *pParent);
    ~PreferencesDialog();

    QStringList pluginNames() const;

private:
    Ui::PreferencesDialog *mGui;

    PluginManager *mPluginManager;

    QStandardItemModel *mModel;

    QMap<PluginInfo::Category, QStandardItem *> mCategoryItems;
    QMap<QStandardItem *, PluginInfo::Category> mItemCategories;
    QMap<QStandardItem *, Preferences::PreferencesWidget *> mItemPreferencesWidgets;
    QMap<Preferences::PreferencesWidget *, QString> mPreferencesWidgetPluginNames;

    PreferencesPluginCategoryWidget *mPluginCategoryWidget;

    QPushButton *mResetAllButton;
    QPushButton *mResetPluginButton;

    QStringList mPluginNames;

    QStandardItem * pluginCategoryItem(const PluginInfo::Category &pCategory);

private slots:
    void on_treeView_collapsed(const QModelIndex &pIndex);

    void on_buttonBox_accepted();

    void updatePreferencesWidget(const QModelIndex &pNewIndex,
                                 const QModelIndex &pOldIndex);

    void resetAll();
    void resetPlugin();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
