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
#pragma once

#include <QGraphicsTextItem>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class SceneItem;
class BGEditorScene;

class BGTextLabelEdit : public QGraphicsTextItem
{
    Q_OBJECT

public:
    BGTextLabelEdit();
    ~BGTextLabelEdit() override;

    void startEdit(SceneItem *item);
    void finishEdit(bool accept = true);

    virtual bool onKeyPressed(BGEditorScene &scene, QKeyEvent *keyEvent);
    virtual bool onKeyReleased(BGEditorScene &scene, QKeyEvent *keyEvent);

Q_SIGNALS:
    void editingStarted(SceneItem *item);
    void editingFinished(SceneItem *item, bool cancelled);

protected:
    bool sceneEvent(QEvent *event) override;

private
    Q_SLOT : void updateGeometry();

private:
    SceneItem *m_item = nullptr;
    QString m_storedText;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR