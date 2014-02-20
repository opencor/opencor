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
// Raw view widget
//==============================================================================

#ifndef RAWVIEWWIDGET_H
#define RAWVIEWWIDGET_H

//==============================================================================

#include "qscintillawidget.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace Ui {
    class RawViewWidget;
}

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

class RawViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit RawViewWidget(QWidget *pParent = 0);
    ~RawViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    QScintillaSupport::QScintillaWidget * editor(const QString &pFileName) const;

private:
    Ui::RawViewWidget *mGui;

    QScintillaSupport::QScintillaWidget *mEditor;
    QMap<QString, QScintillaSupport::QScintillaWidget *> mEditors;

    int mEditorZoomLevel;

private Q_SLOTS:
    void editorZoomLevelChanged();
};

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
