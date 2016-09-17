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
// Preferences dialog
//==============================================================================

#pragma once

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QDialog>
#include <QStyledItemDelegate>

//==============================================================================

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

//==============================================================================

class PreferencesItemDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(PluginManager *pPluginManager, QWidget *pParent);
    ~PreferencesDialog();

private:
    Ui::PreferencesDialog *mGui;

    PluginManager *mPluginManager;

    QStandardItemModel *mModel;

    QMap<PluginInfo::Category, QStandardItem *> mCategoryItems;

    QStandardItem * pluginCategoryItem(const PluginInfo::Category &pCategory);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
