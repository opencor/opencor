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

#include <QSettings>
#include <QVariant>
#include <QWidget>

#include "bgelementeditorscene.h"

namespace Ui {
class BGElementConnectionPropertiesUI;
}

namespace OpenCOR {
namespace BondGraphUI {

class BGEditorScene;
class BGElement;
class BGConnection;

class BGElementConnectionPropertiesUI : public QWidget {
  Q_OBJECT

public:
  explicit BGElementConnectionPropertiesUI(QWidget *parent = nullptr);
  ~BGElementConnectionPropertiesUI();

  void setScene(BGElementEditorScene *scene);

  void doReadSettings(QSettings &settings);
  void doWriteSettings(QSettings &settings);

public Q_SLOTS:
  void updateFromScene(BGEditorScene *scene);

protected:
  void connectSignals(BGEditorScene *scene);
  void onSceneAttached(BGEditorScene *scene);
  void onSceneDetached(BGEditorScene *scene);

protected Q_SLOTS:
  void onSceneChanged();
  void onSelectionChanged();

  void on_ElementColour_activated(const QColor &color);
  void on_ElementShape_activated(QVariant data);
  void on_ElementSizeX_valueChanged(int value);
  void on_ElementSizeY_valueChanged(int value);
  void on_ElementSizeSwitch_toggled(bool on);
  void on_StrokeColour_activated(const QColor &color);
  void on_StrokeStyle_activated(QVariant data);
  void on_StrokeSize_valueChanged(double value);
  void on_zValueSpinBox_valueChanged(int value);

  void on_ConnectionColour_activated(const QColor &color);
  void on_ConnectionWeight_valueChanged(double value);
  void on_ConnectionStyle_activated(QVariant data);
  void on_ConnectionDirection_activated(QVariant data);
  void on_ConnectionType_activated(QVariant data);

  void on_LabelFont_activated(const QFont &font);
  void on_LabelColour_activated(const QColor &color);
  void on_LabelFontSize_valueChanged(int value);
  void on_LabelFontBold_toggled(bool on);
  void on_LabelFontItalic_toggled(bool on);
  void on_LabelFontUnderline_toggled(bool on);

private:
  void setElementAttribute(const QByteArray &attrId, const QVariant &v);
  void setConnectionAttribute(const QByteArray &attrId, const QVariant &v);

  BGElementEditorScene *m_scene;
  bool m_updateLock;

  BGElement *m_elementFactory;
  BGConnection *m_connectionFactory;

  Ui::BGElementConnectionPropertiesUI *ui;
};

} // namespace BondGraphUI
} // namespace OpenCOR