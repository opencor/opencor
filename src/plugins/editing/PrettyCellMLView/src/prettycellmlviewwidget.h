/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Pretty CellML view widget
//==============================================================================

#ifndef PRETTYCELLMLVIEWWIDGET_H
#define PRETTYCELLMLVIEWWIDGET_H

//==============================================================================

#include "cliutils.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace Ui {
    class PrettyCellmlViewWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreCellMLEditing {
    class CoreCellmlEditingWidget;
}   // namespace CoreCellMLEditing

//==============================================================================

namespace Editor {
    class EditorWidget;
}   // namespace Editor

//==============================================================================

namespace PrettyCellMLView {

//==============================================================================

class PrettyCellmlViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit PrettyCellmlViewWidget(QWidget *pParent);
    ~PrettyCellmlViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    Editor::EditorWidget * editor(const QString &pFileName) const;

    virtual QList<QWidget *> statusBarWidgets() const;

private:
    Ui::PrettyCellmlViewWidget *mGui;

    CoreCellMLEditing::CoreCellmlEditingWidget *mEditingWidget;
    QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *> mEditingWidgets;

    QIntList mEditingWidgetSizes;
    int mEditorZoomLevel;

    bool mViewerOptimiseFontSizeEnabled;
    bool mViewerSubscriptsEnabled;
    bool mViewerGreekSymbolsEnabled;
    bool mViewerDigitGroupingEnabled;

private Q_SLOTS:
    void splitterMoved();
    void editorZoomLevelChanged(const int &pZoomLevel);

    void optimiseFontSizeChanged(const bool &pEnabled);
    void subscriptsChanged(const bool &pEnabled);
    void greekSymbolsChanged(const bool &pEnabled);
    void digitGroupingChanged(const bool &pEnabled);
};

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
