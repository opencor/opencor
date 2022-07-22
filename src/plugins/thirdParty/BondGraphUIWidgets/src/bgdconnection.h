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

#include "bgconnection.h"
#include "thirdparty/json.hpp"

namespace OpenCOR {
namespace BondGraphUI {

class BGDConnection : public BGConnection {
public:
  BGDConnection(QGraphicsItem *parent = Q_NULLPTR);

  void setBendFactor(int bf);
  static QByteArray TYPE() { return "BGDConnection"; }
  virtual QByteArray typeId() const { return "BGDConnection"; }
  virtual QByteArray classId() const { return "connection"; }
  virtual QByteArray superClassId() const { return BGConnection::classId(); }

  virtual SceneItem *create() const { return new BGDConnection(parentItem()); }
  BGConnection *clone();

  virtual QPointF getLabelCenter() const { return m_controlPoint; }
  int getBendFactor() const { return m_bendFactor; }
  // serialization
  virtual bool storeTo(QDataStream &out, quint64 version64) const;
  virtual bool restoreFrom(QDataStream &out, quint64 version64);

  friend void to_json(nlohmann::json &j, const BGDConnection &p);
  friend void from_json(const nlohmann::json &j, BGDConnection &p);

  // transformations
  virtual void transform(const QRectF &oldRect, const QRectF &newRect,
                         double xc, double yc, bool changeSize,
                         bool changePos) override {
    Q_UNUSED(oldRect);
    Q_UNUSED(newRect);
    Q_UNUSED(xc);
    Q_UNUSED(yc);
    Q_UNUSED(changeSize);
    Q_UNUSED(changePos);
  }

protected:
  // override
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget = Q_NULLPTR);
  virtual void updateLabelPosition();

  // callbacks
  virtual void onParentGeometryChanged();

protected:
  int m_bendFactor;
  QPointF m_controlPoint, m_controlPos;
};
} // namespace BondGraphUI
} // namespace OpenCOR