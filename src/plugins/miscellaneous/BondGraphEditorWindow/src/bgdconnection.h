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
namespace BondGraphEditorWindow {

class BGDConnection : public BGConnection
{
public:
    BGDConnection(QGraphicsItem *parent = Q_NULLPTR);
    BGConnection *clone() override;

    void setBendFactor(int bendfactor);
    static QByteArray TYPE();
    virtual QByteArray typeId() const override;
    virtual QByteArray classId() const override;
    virtual QByteArray superClassId() const override;
    virtual SceneItem *create() const override;
    virtual QPointF getLabelCenter() const override;
    int getBendFactor() const;

    // serialization
    virtual bool storeTo(QDataStream &out, quint64 version64) const override;
    virtual bool restoreFrom(QDataStream &out, quint64 version64) override;

    friend void to_json(nlohmann::json &json_, const BGDConnection &pconn);
    friend void from_json(const nlohmann::json &json_, BGDConnection &pconn);

    // transformations
    void transform(const QRectF &oldRect, const QRectF &newRect,
                   double xc, double yc, bool changeSize,
                   bool changePos) override;

protected:
    // override
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = Q_NULLPTR) override;
    virtual void updateLabelPosition() override;

    // callbacks
    virtual void onParentGeometryChanged() override;

protected:
    int m_bendFactor;
    QPointF m_controlPoint, m_controlPos;
};
} // namespace BondGraphEditorWindow
} // namespace OpenCOR
