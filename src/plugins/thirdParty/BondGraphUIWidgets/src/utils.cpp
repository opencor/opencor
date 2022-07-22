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
#include "utils.h"

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QPoint>
#include <QPointF>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include <cmath>

namespace OpenCOR {
namespace BondGraphUI {
namespace Utils {

bool CellMLVariableFilter::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
  QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
  // QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
  // Will return true if the parent's name matched the term
  return (sourceModel()->data(index0).toString().contains(filterRegExp()) ||
          sourceModel()->data(index1).toString().contains(filterRegExp()) ||
          sourceParent.data().toString().contains(filterRegExp()));
}

QVariant textToVariant(const QString &text, int type) {
  switch (type) {
  case QMetaType::QStringList:
    return text.split('|', QString::SkipEmptyParts);

  case QVariant::Int:
    return text.toInt();

  case QVariant::Double:
    return text.toDouble();

  case QMetaType::Float:
    return text.toFloat();

  case QVariant::Bool:
    if (text.toLower() == "true")
      return true;
    else
      return false;

  case QVariant::Color:
    return QColor(text);

  case QVariant::Font: {
    QFont f;
    f.fromString(text);
    return f;
  }

  default:
    return text; // string
  }
}

QString variantToText(const QVariant &v, int type) {
  if (type < 0)
    type = v.type();

  switch (type) {
  case QVariant::Point:
    return QString("%1;%2").arg(v.toPoint().x()).arg(v.toPoint().y());

  case QVariant::PointF:
    return QString("%1;%2").arg(v.toPointF().x()).arg(v.toPointF().y());

  case QVariant::Size:
    return QString("%1:%2").arg(v.toSize().width()).arg(v.toSize().height());

  case QVariant::SizeF:
    return QString("%1:%2").arg(v.toSizeF().width()).arg(v.toSizeF().height());

  case QVariant::Bool:
    return v.toBool() ? "true" : "false";

  case QVariant::Double:
    return QString::number(v.toDouble(), 'f', 4);

  case QMetaType::Float:
    return QString::number(v.value<float>(), 'f', 4);

  case QMetaType::QStringList:
    return v.toStringList().join('|');

  default:;
    return v.toString();
  }
}

Qt::PenStyle textToPenStyle(const QString &text, Qt::PenStyle def) {
  static QMap<QString, Qt::PenStyle> s_penStyles = {
      {"none", Qt::NoPen},          {"solid", Qt::SolidLine},
      {"dashed", Qt::DashLine},     {"dotted", Qt::DotLine},
      {"dashdot", Qt::DashDotLine}, {"dashdotdot", Qt::DashDotDotLine}};

  if (s_penStyles.contains(text))
    return s_penStyles[text];
  else
    return def;
}

QString penStyleToText(int style) {
  switch (style) {
  case Qt::SolidLine:
    return QStringLiteral("solid");
  case Qt::DashLine:
    return QStringLiteral("dashed");
  case Qt::DotLine:
    return QStringLiteral("dotted");
  case Qt::DashDotLine:
    return QStringLiteral("dashdot");
  case Qt::DashDotDotLine:
    return QStringLiteral("dashdotdot");
  default:
    return QStringLiteral("none");
  }
}

QString visToString(const QSet<QByteArray> &visIds) {
  return visIds.toList().join('|');
}

QSet<QByteArray> visFromString(const QString &text) {
  return text.toUtf8().split('|').toSet();
}

QStringList byteArraySetToStringList(const QSet<QByteArray> &ids) {
  QStringList sl;
  for (const auto &id : ids)
    sl << id;
  return sl;
}

QString pointsToString(const QList<QPointF> &points) {
  QString s;
  QTextStream ds(&s, QIODevice::WriteOnly);
  for (const auto &p : points) {
    float x = p.x();
    float y = p.y();
    ds << x << " " << y << " ";
  }

  return s;
}

QList<QPointF> pointsFromString(const QString &text) {
  QList<QPointF> pl;

  QString str(text.trimmed());
  if (str.size()) {
    float x = 0, y = 0;
    QTextStream ds(&str);
    while (!ds.atEnd()) {
      ds >> x;
      ds >> y; // to do: check validity
      pl.append(QPointF(x, y));
    }
  }

  return pl;
}

QPointF closestIntersection(const QLineF &line, const QPolygonF &endPolygon) {
  QPointF intersectPoint;
  QPointF p1 = endPolygon.first();
  QPointF p2;

  for (int i = 1; i < endPolygon.count(); ++i) {
    p2 = endPolygon.at(i);
    QLineF polyLine = QLineF(p1, p2);
    QLineF::IntersectType intersectType =
        polyLine.intersect(line, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection)
      return intersectPoint;

    p1 = p2;
  }

  return QPointF();
}

QString cutLastSuffix(const QString &fileName) {
  int idx = fileName.lastIndexOf(".");
  if (idx < 0)
    return fileName;
  else
    return fileName.left(idx);
}

QRectF getBoundingRect(const QList<QGraphicsItem *> &items) {
  QRectF r;

  for (const auto item : items)
    r |= item->sceneBoundingRect();

  return r;
}

QLineF extendLine(const QLineF &line, float fromStart, float fromEnd) {
  QPointF v(line.p2().x() - line.p1().x(), line.p2().y() - line.p1().y());

  float l = std::sqrt(v.x() * v.x() + v.y() * v.y());

  v.setX(v.x() / l);
  v.setY(v.y() / l);

  return QLineF(
      line.p1().x() - v.x() * fromStart, line.p1().y() - v.y() * fromStart,
      line.p2().x() - v.x() * fromEnd, line.p2().y() - v.y() * fromEnd);
}

QMap<QString, QList<QString>> getCellMLOutVariables(QString filename,
                                                    QString *timeVar) {

  QDomDocument xmlBOM;
  QMap<QString, QList<QString>> result;
  QFile f(filename);
  if (!f.open(QIODevice::ReadOnly)) {
    // Error while loading file
    throw "Error while loading file " + filename;
  }
  // Set data into the QDomDocument before processing
  xmlBOM.setContent(&f);
  f.close();

  QDomElement Component = xmlBOM.documentElement().firstChild().toElement();
  if (timeVar != nullptr) {
    QString str;
    QTextStream stream(&str);

    xmlBOM.documentElement().save(stream, 4 /*indent*/);
    auto bvi = str.indexOf("bvar");
    if (bvi != -1) {
      auto bve = str.indexOf("</bvar>", bvi + 4);
      QString bvar = str.mid(bvi + 5, bve - bvi - 5)
                         .split(' ', QString::SkipEmptyParts)
                         .join("")
                         .replace("\n", "")
                         .replace("<ci>", "")
                         .replace("</ci>", "");
      // qDebug() << Q_FUNC_INFO<<"\t"<<bvar;
      *timeVar = bvar;
    }
  }

  QMap<QString, QList<QString>> definedUnits;
  // Loop while there is a child
  while (!Component.isNull()) {
    // Map dimension definitions too
    if (Component.tagName() == "units" || Component.tagName() == "UNITS") {
      QString uname = Component.attribute("name");
      QDomElement Child = Component.firstChild().toElement();
      QList<QString> udef;
      while (!Child.isNull()) {
        if (Child.tagName() == "unit") {
          udef.append(Child.attribute("units"));
          udef.append(Child.attribute("exponent", "1"));
          // udef.append(Child.attribute("multiplier", ""));
          // udef.append(Child.attribute("prefix", ""));
        }
        // Next child
        Child = Child.nextSibling().toElement();
      }
      definedUnits[uname] = udef;
    }

    // Check if the child tag name is COMPONENT
    if (Component.tagName() == "component" ||
        Component.tagName() == "COMPONENT") {

      // Read and display the component ID
      QString ID = Component.attribute("name");

      // Get the first child of the component
      QDomElement Child = Component.firstChild().toElement();

      while (!Child.isNull()) {
        if (Child.tagName() == "variable") {
          QString name;
          QString interface;
          QString dimensions;
          QString initialValue;
          // Read Name and value
          if (Child.attribute("public_interface") == "out") {
            name = Child.attribute("name");
            dimensions = Child.attribute("units");
            initialValue = Child.attribute("initial_value", "");
            if (dimensions != "second") {
              QList<QString> vals;
              vals.append(name);
              vals.append(ID);
              vals.append(dimensions);
              vals.append(initialValue);
              if (definedUnits.contains(dimensions)) {
                auto ut = definedUnits[dimensions];
                QString unitDef;
                int c = 0;
                for (auto &u : ut) {
                  unitDef += u;
                  c++;
                  if (c % 2 == 0) {
                    unitDef += "*";
                  } else {
                    unitDef += "^";
                  }
                }
                // Append one to handle * at the end
                vals.append(unitDef + "1");
              }
              result[ID + ":" + name] = vals;
            }
          }
        }

        // Next child
        Child = Child.nextSibling().toElement();
      }
    }

    // Next component
    Component = Component.nextSibling().toElement();
  }

  return result;
}

} // namespace Utils
} // namespace BondGraphUI
} // namespace OpenCOR