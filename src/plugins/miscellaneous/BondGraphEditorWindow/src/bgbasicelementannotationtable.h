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

#include <QAbstractItemDelegate>
#include <QGraphicsItem>
#include <QSettings>
#include <QVariant>
#include <QWidget>

namespace Ui {
class BGBasicElementAnnotationTable;
};

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;
class BGElement;
class BGElementEditorScene;
class QJsonTableModel;
class ParameterDelegate;

class BGBasicElementAnnotationTable : public QWidget
{
    Q_OBJECT

public:
    explicit BGBasicElementAnnotationTable(QWidget *parent = nullptr,
                                           BGElementEditorScene *scene = nullptr);
    ~BGBasicElementAnnotationTable() override;

    void setScene(BGElementEditorScene *scene);

    void doReadSettings(QSettings &settings);
    void doWriteSettings(QSettings &settings);

Q_SIGNALS:
    void elementUpdated(BGElement *elem);

public Q_SLOTS:
    void updateFromScene(BGEditorScene *scene);
    void updateLatex(const QString &text);
    void itemDeleted(QGraphicsItem *item);
    void onSelectionChanged();
    void flush();

protected:
    void onSceneAttached(BGEditorScene *scene);
    void onSceneDetached(BGEditorScene *scene);

protected Q_SLOTS:
    void onSceneChanged();
    void setElementAsProxy();

private:
    void setElementAttribute(const QByteArray &attrId, const QVariant &value);
    void setConnectionAttribute(const QByteArray &attrId, const QVariant &value);

    BGElementEditorScene *m_scene;
    bool m_updateLock = false;

    BGElement *m_currentElement = nullptr;
    QJsonTableModel *m_parameterModel;
    ParameterDelegate *m_parameterDelegate;
    Ui::BGBasicElementAnnotationTable *ui;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
