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
// Raw Text view widget
//==============================================================================

#pragma once

//==============================================================================

#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace EditorWidget {
    class EditorWidget;
} // namespace EditorWidget

//==============================================================================

namespace RawTextView {

//==============================================================================

class RawTextViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit RawTextViewWidget(QWidget *pParent);

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void retranslateUi() override;

    void initialize(const QString &pFileName, bool pUpdate = true);
    void finalize(const QString &pFileName);

    void fileSaved(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const;

    QWidget * widget(const QString &pFileName) override;

    QList<QWidget *> statusBarWidgets() const override;

private:
    bool mNeedLoadingSettings = true;
    QString mSettingsGroup;

    EditorWidget::EditorWidget *mEditor = nullptr;
    QMap<QString, EditorWidget::EditorWidget *> mEditors;
};

//==============================================================================

} // namespace RawTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
