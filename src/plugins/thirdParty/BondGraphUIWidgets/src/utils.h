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

#include <QByteArray>
#include <QGraphicsItem>
#include <QLineF>
#include <QMap>
#include <QPen>
#include <QPointF>
#include <QPolygonF>
#include <QSet>
#include <QSortFilterProxyModel>
#include <QString>
#include <QVariant>

namespace OpenCOR {
namespace BondGraphUI {

namespace Utils {

QString variantToText(const QVariant &v, int type = -1);
QVariant textToVariant(const QString &text, int type = QVariant::String);

Qt::PenStyle textToPenStyle(const QString &text, Qt::PenStyle def = Qt::NoPen);
QString penStyleToText(int style);

QString visToString(const QSet<QByteArray> &visIds);
QSet<QByteArray> visFromString(const QString &text);
QStringList byteArraySetToStringList(const QSet<QByteArray> &visIds);

QString pointsToString(const QList<QPointF> &points);
QList<QPointF> pointsFromString(const QString &text);

template <class X> void insertUnique(X &dest, const X &from);

QPointF closestIntersection(const QLineF &line, const QPolygonF &with);

QString cutLastSuffix(const QString &fileName);

QRectF getBoundingRect(const QList<QGraphicsItem *> &items);

QLineF extendLine(const QLineF &line, float fromStart, float fromEnd);

QMap<QString, QList<QString>> getCellMLOutVariables(QString filename,
                                                    QString *timeVar = nullptr);

class CellMLVariableFilter : public QSortFilterProxyModel {
public:
  CellMLVariableFilter(QObject *parent = nullptr)
      : QSortFilterProxyModel(parent){};
  virtual ~CellMLVariableFilter(){};

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;
};

template <class X> void insertUnique(X &dest, const X &from) {
  for (auto it = from.constBegin(); it != from.constEnd(); ++it) {
    if (!dest.contains(it.key()))
      dest[it.key()] = it.value();
  }
}
} // namespace Utils
} // namespace BondGraphUI
} // namespace OpenCOR