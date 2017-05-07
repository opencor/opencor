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
// SED-ML Editing view widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "sedmleditingviewglobal.h"
#include "splitterwidget.h"

//==============================================================================

#include <QString>

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace EditorWidget {
    class EditorListItem;
    class EditorListWidget;
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace SEDMLEditingView {

//==============================================================================

class SEDMLEDITINGVIEW_EXPORT SedmlEditingViewWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:
    explicit SedmlEditingViewWidget(const QString &pContents,
                                    const bool &pReadOnly, QsciLexer *pLexer,
                                    QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(SedmlEditingViewWidget *pSedmlEditingViewWidget);

    EditorWidget::EditorWidget * editorWidget() const;
    EditorWidget::EditorListWidget * editorListWidget() const;

    QIntList editingWidgetSizes() const;

private:
    EditorWidget::EditorWidget *mEditorWidget;
    EditorWidget::EditorListWidget *mEditorListWidget;

    QIntList mEditingWidgetSizes;

private slots:
    void splitterMoved();
    void itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem);
};

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
