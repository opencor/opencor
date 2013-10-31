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
// Raw CellML view widget
//==============================================================================

#ifndef RAWCELLMLVIEWWIDGET_H
#define RAWCELLMLVIEWWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class RawCellmlViewWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
}   // namespace Core

//==============================================================================

namespace QScintillaSupport {
    class QScintilla;
}   // namespace QScintillaSupport

//==============================================================================

namespace Viewer {
    class ViewerWidget;
}   // namespace Viewer

//==============================================================================

namespace RawCellMLView {

//==============================================================================

class RawCellmlViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit RawCellmlViewWidget(QWidget *pParent = 0);
    ~RawCellmlViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    bool isManaged(const QString &pFileName) const;
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);

private:
    Ui::RawCellmlViewWidget *mGui;

    Core::BorderedWidget *mBorderedViewer;
    Viewer::ViewerWidget *mViewer;

    Core::BorderedWidget *mBorderedEditor;
    QMap<QString, Core::BorderedWidget *> mBorderedEditors;

    int mBorderedViewerHeight;
    int mBorderedEditorHeight;

private Q_SLOTS:
    void splitterMoved();
};

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
