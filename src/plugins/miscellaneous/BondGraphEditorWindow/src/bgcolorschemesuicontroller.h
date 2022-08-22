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
#include <QList>
#include <QMenu>
#include <QObject>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

class BGColorSchemesUIController : public QObject
{
    Q_OBJECT
public:
    explicit BGColorSchemesUIController(QObject *parent = nullptr);

    void setScene(BGEditorScene *scene);
    QMenu *getSchemesMenu();

Q_SIGNALS:
    void colorSchemeApplied(BGEditorScene *scene);

private Q_SLOTS:
    void onMenuTriggered(QAction *action);

private:
    QMenu m_menu;
    BGEditorScene *m_scene = nullptr;

    struct Scheme {
        QString name;
        QColor bgColor, gridColor;
        QColor nodeColor, nodeStrokeColor, nodeLabelColor;
        QColor edgeColor, edgeLabelColor;
    };

    QList<Scheme> m_schemes;

    void addScheme(const Scheme &scheme);
    void applyScheme(const Scheme &scheme);
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
