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
#include "bgannotationviewmetadataeditdetailsui.h"
#include "bgconnection.h"
#include "bgdconnection.h"
#include "bgelement.h"

#include <QBrush>
#include <QByteArray>
#include <QCoreApplication>

#include <QEvent>
#include <QPen>
#include <QSet>
#include <QSettings>
#include <QtMath>

#include "thirdparty/jkqtlatex/jkqtmathtext.h"
// test
#include <QGraphicsDropShadowEffect>

namespace OpenCOR {
namespace BondGraphEditorWindow {
////////////////////////////////////////////////////////////////////
/// \brief BGElement::BGElement

BGElement::BGElement(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
    // no selection frame
    setItemFlag(IF_FramelessSelection);

    // default size
    resize(20);

    // default element flags
    m_Flags = NF_OrphanAllowed;

    // default flags: movable & selectable
    auto flags = ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemUsesExtendedStyleOption;
    setFlags(flags);

    // accept hovers
    setAcceptHoverEvents(true);
    // setFiltersChildEvents(true);

    // cache
    setCacheMode(DeviceCoordinateCache);

    // label
    m_labelItem = new QGraphicsSimpleTextItem(this);
    m_labelItem->setFlags(0);
    m_labelItem->setCacheMode(DeviceCoordinateCache);
    m_labelItem->setPen(Qt::NoPen);
    m_labelItem->setAcceptedMouseButtons(Qt::NoButton);
    m_labelItem->setAcceptHoverEvents(false);

    // QSettings *settings = new QSettings(QCoreApplication::organizationName(),
    //                                     QCoreApplication::applicationName());
    QSettings settings;
    minDim = settings.value("BondGraphEditorWindow/bgelement/maxDim", 50).toInt();
    // Cover standard elements
    itemColours["OneJunction"] =
        settings.value("BondGraphEditorWindow/bgelement/OneJunction", QColor(Qt::green))
            .value<QColor>();
    itemColours["ZeroJunction"] =
        settings.value("BondGraphEditorWindow/bgelement/ZeroJunction", QColor(Qt::red))
            .value<QColor>();
    itemColours["Resistance"] =
        settings.value("BondGraphEditorWindow/bgelement/Resistance", QColor("#6666ff"))
            .value<QColor>();
    itemColours["Capacitance"] =
        settings.value("BondGraphEditorWindow/bgelement/Capacitance", QColor("#00cc00"))
            .value<QColor>();
    itemColours["Inductance"] =
        settings.value("BondGraphEditorWindow/bgelement/Inductance", QColor(Qt::magenta))
            .value<QColor>();
    itemColours["PotentialSource"] =
        settings
            .value("BondGraphEditorWindow/bgelement/PotentialSource", QColor(Qt::darkBlue))
            .value<QColor>();
    itemColours["FlowSource"] =
        settings.value("BondGraphEditorWindow/bgelement/FlowSource", QColor(Qt::darkRed))
            .value<QColor>();
    itemColours["Gyrator"] =
        settings.value("BondGraphEditorWindow/bgelement/Gyrator", QColor("#ff9900"))
            .value<QColor>();
    itemColours["Transformer"] =
        settings.value("BondGraphEditorWindow/bgelement/Gyrator", QColor(Qt::darkGray))
            .value<QColor>();
    itemColours["Reaction"] =
        settings.value("BondGraphEditorWindow/bgelement/Reaction", QColor(Qt::darkCyan))
            .value<QColor>();
    itemColours["Concentration"] =
        settings
            .value("BondGraphEditorWindow/bgelement/Concentration", QColor(Qt::darkBlue))
            .value<QColor>();
    itemColours["UserDefined"] =
        settings.value("BondGraphEditorWindow/bgelement/UserDefined", QColor("#9933ff"))
            .value<QColor>();
    itemColours["CellML"] =
        settings.value("BondGraphEditorWindow/bgelement/UserDefined", QColor("#9933ff"))
            .value<QColor>();
    itemColours["PortHamiltonian"] =
        settings.value("BondGraphEditorWindow/bgelement/PortHamiltonian", QColor("#ccff00"))
            .value<QColor>();
    itemColours["Region"] =
        settings.value("BondGraphEditorWindow/bgelement/Region", QColor(Qt::transparent))
            .value<QColor>();
    // delete settings;
}

BGElement::~BGElement()
{
    for (BGPort *port : m_ports) {
        port->onParentDeleted();
    }

    for (BGConnection *conn : m_connections) {
        conn->onElementDeleted(this);
    }
}

// override

QByteArray BGElement::TYPE()
{
    return "BGElement";
}
QByteArray BGElement::typeId() const
{
    return "BGElement";
}

int BGElement::bgElementFlags() const
{
    return m_Flags;
}
void BGElement::setBGElementFlags(int flag)
{
    m_Flags = flag;
}
void BGElement::setBGElementFlag(int flag)
{
    m_Flags |= flag;
}
void BGElement::resetBGElementFlag(int flag)
{
    m_Flags &= ~flag;
}

QString BGElement::getDisplayName() const
{
    return mDisplayName;
}

SceneItem *BGElement::create() const
{
    return new BGElement(parentItem());
}

QSizeF BGElement::getSize() const
{
    return rect().size();
}
int BGElement::getMinDim() const
{
    return minDim;
}

QByteArray BGElement::classId() const
{
    return "element";
}

QByteArray BGElement::superClassId() const
{
    return SceneItem::classId();
}

int BGElement::getMaxPorts() const
{
    return maxPorts;
};

QSet<BGConnection *> BGElement::getConnections() const
{
    return m_connections;
}

bool BGElement::allowCircledConnection() const
{
    return false;
}

QString BGElement::createNewId() const
{
    // static const QString tmpl("BGEl%1");
    return createUniqueId<BGElement>(m_shortname);
}

void BGElement::copyDataFrom(SceneItem *from)
{
    SceneItem::copyDataFrom(from);

    BGElement *fromNode = dynamic_cast<BGElement *>(from);
    if (fromNode) {
        // shape & position
        resize(fromNode->getSize());
        setPos(fromNode->pos());
        setZValue(fromNode->zValue());

        // ports
        qDeleteAll(m_ports);
        m_ports.clear();
        for (auto it = fromNode->m_ports.begin(); it != fromNode->m_ports.end();
             ++it) {
            BGPort *port = new BGPort(this);
            port->copyDataFrom(**it);
            m_ports[it.key()] = port;
        }

        updatePortsLayout();
    }

    setAttribute("shape", "square");
    updateCachedItems();
}

SceneItem *BGElement::clone()
{
    SceneItem *item = create();
    BGElement *element = dynamic_cast<BGElement *>(item);

    if (scene())
        scene()->addItem(element);

    element->copyDataFrom(this);

    return item;
}

// transformations

void BGElement::transform(const QRectF &old_rect, const QRectF &new_rect,
                          double xc, double yc, bool change_size,
                          bool change_pos)
{
    double w = getSize().width();
    double h = getSize().height();
    double wc = w;
    double hc = h;

    if (change_size) {
        wc = (w * xc);
        hc = (h * yc);

        setSize(wc, hc);
    } else {
        w = h = wc = hc = 0.0;
    }

    if (change_pos) {
        double dx = x() - w / 2;
        double dy = y() - h / 2;
        double xp = (dx - old_rect.left()) * xc + new_rect.left() + wc / 2;
        double yp = (dy - old_rect.top()) * yc + new_rect.top() + hc / 2;

        setPos(xp, yp);
    }
}

// attributes

void BGElement::setSize(float w, float h)
{
    if (w < minDim || h < minDim)
        return;
    setAttribute("size", QSizeF(w, h));
    if (!m_iconCache.isNull()) {
        m_iconCache = m_icon.scaled(QSize(w, h), Qt::KeepAspectRatio);
        const QSize ms = m_iconCache.size();
        const QRectF r = Shape::boundingRect();

        iconLeft = r.topLeft().x() + (r.height() - ms.height()) / 2;
        iconTop = r.topLeft().y() + (r.width() - ms.width()) / 2;
    }
}

bool BGElement::hasLocalAttribute(const QByteArray &attr_id) const
{
    if (attr_id == "width" || attr_id == "height" || attr_id == "pos" || attr_id == "x" || attr_id == "y" || attr_id == "z")
        return true;

    return SceneItem::hasLocalAttribute(attr_id);
}

void BGElement::setDisplayName(QString name)
{
    setItemStateFlag(IS_Attribute_Changed);

    mDisplayName = name;
    setIcon(getRenderedLatex());
    m_iconCache = m_icon;
    updateCachedItems();

    update();
}

bool BGElement::setAttribute(const QByteArray &attr_id, const QVariant &v)
{
    setItemStateFlag(IS_Attribute_Changed);

    update();

    if (attr_id == "shape") {
        SceneItem::setAttribute(attr_id, v);
        updateCachedItems();
        return true;
    }

    // mapped attributes
    if (attr_id == "size") {
        if (v.type() == QVariant::Size || v.type() == QVariant::SizeF) {
            QSizeF sp = v.toSizeF();
            if (!sp.isNull()) {
                SceneItem::setAttribute(attr_id, sp);
                resize(sp);
                updateCachedItems();
                return true;
            }
            return false;
        }

        float s = v.toFloat();
        if (s > 0) {
            SceneItem::setAttribute(attr_id, QSizeF(s, s));
            resize(s);
            updateCachedItems();
            return true;
        }

        return false;
    }

    if (attr_id == "width") {
        float s = v.toFloat();
        QSizeF sf = getSize();
        SceneItem::setAttribute("size", QSizeF(s, sf.height()));
        resize(s, sf.height());
        return true;
    }

    if (attr_id == "height") {
        float s = v.toFloat();
        QSizeF sf = getSize();
        SceneItem::setAttribute("size", QSizeF(sf.width(), s));
        resize(sf.width(), s);
        return true;
    }

    if (attr_id == "x") {
        setX(v.toDouble());
        return true;
    }

    if (attr_id == "y") {
        setY(v.toDouble());
        return true;
    }

    if (attr_id == "z") {
        setZValue(v.toDouble());
        return true;
    }

    if (attr_id == "pos") {
        setPos(v.toPointF());
        return true;
    }

    return SceneItem::setAttribute(attr_id, v);
}

bool BGElement::removeAttribute(const QByteArray &attr_id)
{
    if (SceneItem::removeAttribute(attr_id)) {
        updateCachedItems();

        return true;
    } else
        return false;
}

QVariant BGElement::getAttribute(const QByteArray &attr_id) const
{
    // mapped attributes
    if (attr_id == "x") {
        return x();
    }

    if (attr_id == "y") {
        return y();
    }

    if (attr_id == "z") {
        return zValue();
    }

    if (attr_id == "pos") {
        return pos();
    }

    // virtual attributes
    if (attr_id == "degree") {
        return m_connections.size();
    }

    return SceneItem::getAttribute(attr_id);
}

bool BGElement::isProxy()
{
    return m_proxy == -1;
}

bool BGElement::canBeProxy()
{
    return m_proxy < 1;
}

void BGElement::setAsProxy()
{
    m_proxy = -1;
}

void createPorts(BGElement *elem, nlohmann::json &port_desc)
{
    // Create a port for each key
    const int alignments[] = {
        Qt::AlignLeft + Qt::AlignVCenter, Qt::AlignRight + Qt::AlignVCenter,
        Qt::AlignTop + Qt::AlignLeft, Qt::AlignTop + Qt::AlignHCenter,
        Qt::AlignTop + Qt::AlignRight, Qt::AlignBottom + Qt::AlignLeft,
        Qt::AlignBottom + Qt::AlignHCenter, Qt::AlignBottom + Qt::AlignRight
    };
    int acount = 0;
    for (auto &pn : port_desc.items()) {
        std::string key = pn.key();
        QString pname(key.c_str());
        auto val = pn.value();
        if (val.contains("dir")) {
            key = val["dir"];
        }
        auto nPort = elem->addPort(pname.toUtf8());
        if (key == "in") {
            nPort->setAlignment(alignments[0]);
            QColor col("blue");
            nPort->setColor(col);
            acount++;
            nPort->setDirection(-1);
        } else if (key == "out") {
            nPort->setAlignment(alignments[1]);
            QColor col("red");
            nPort->setColor(col);
            acount++;
            nPort->setDirection(1);
        } else {
            nPort->setAlignment(alignments[acount]);
            acount++;
        }
    }
}

nlohmann::json BGElement::setBGElementDefinitions(QString json)
{
    // Parse json doc, set number of ports and shape
    nlohmann::json mDefinition = nlohmann::json::parse(json.toStdString());
    // Ports depending on class type
    std::string classType = mDefinition["class"];

    setClassType(QString::fromStdString(classType));
    std::string namePrefix = mDefinition["shortname"];
    namePrefix = namePrefix + "_%1";
    m_shortname = QString::fromStdString(namePrefix);
    // Create my id
    m_id = createNewId();
    mDisplayName = "$" + m_id + "$";
    setDisplayName(m_id);
    std::string type = mDefinition["type"];
    m_type = QString(type.c_str());
    setIcon(getRenderedLatex());
    auto portDesc = mDefinition["ports"];

    if (classType == "passive") {
        // Handle reactions which have 2 ports
        if (portDesc.size() != 2) {
            auto port = addPort();
            port->setAlignment(Qt::AlignBottom + Qt::AlignHCenter);
        } else {
            // Handle reactions which have 2 ports
            maxPorts = 2;
            createPorts(this, portDesc);
        }
    } else if (classType == "junction") {
        // Get the number of ports
        maxPorts = 2;
        if (portDesc.contains("limit")) {
            auto port = addPort("", QPointF(0, 0));
            port->setDeleteFlag(false);
            port->setAlignment(Qt::AlignCenter);
            port->setColor(QColor(Qt::transparent));

            maxPorts = 100;            // 0 or 1 junction
            setAttribute("shape", ""); // Disc
            recalculateShape();
        } else {
            // Create a port for each key
            createPorts(this, portDesc);
        }
    } else if (classType == "annotation") {
        showLabel(true);
    }
    // else if (classType == "userdefined") {

    // }
    // Create json with annotation information
    m_annotation["ElementDefinition"] = mDefinition;
    auto annotation = nlohmann::json::object();
    for (const QString prop :
         BGAnnotationViewMetadataEditDetailsUI::m_supportedAnnotations_static) {
        annotation[prop.toStdString()] = nlohmann::json::array();
    }
    m_annotation["Annotation"] = annotation;
    // States and parameters
    nlohmann::json spjson = nlohmann::json::array();
    auto &edef = m_annotation["ElementDefinition"];
    if (edef.contains("states")) {
        auto &param = edef["states"];
        for (auto rel : param.items()) {
            nlohmann::json obj;
            obj["name"] = rel.key();
            nlohmann::json &value = rel.value();
            for (auto el : value.items()) {
                obj[el.key()] = el.value();
            }
            obj["isState"] = true;
            spjson.push_back(obj);
        }
    }

    if (edef.contains("parameters")) {
        auto &param = edef["parameters"];

        for (auto rel : param.items()) {
            nlohmann::json obj;
            obj["name"] = rel.key();
            nlohmann::json &value = rel.value();
            for (auto el : value.items()) {
                obj[el.key()] = el.value();
            }
            obj["isState"] = false;
            spjson.push_back(obj);
        }
    }
    m_annotation["statesAndParameters"] = spjson;
    m_annotation["constitutive_relations"] = std::string("");
    if (edef.contains("constitutive_relations")) {
        auto jd = edef["constitutive_relations"];
        QString res = QString::fromStdString(jd.dump());
        m_annotation["constitutive_relations"] = res.replace("[", "")
                                                     .replace("]", "")
                                                     .replace("\"", "")
                                                     .replace(",", "\n")
                                                     .toStdString();
    }

    m_annotationCache = QString::fromStdString(m_annotation.dump()).toUtf8();
    return m_annotation;
}

nlohmann::json &BGElement::getJson()
{
    return m_annotation;
}

nlohmann::json &BGElement::getStateParameterJson()
{
    return m_annotation["statesAndParameters"];
}

void BGElement::setStateParameterJson(nlohmann::json &json)
{
    m_annotation["statesAndParameters"] = json;
    m_annotationCache = QString::fromStdString(json.dump()).toUtf8();
}

void BGElement::setJson(nlohmann::json &json)
{
    if (getClassType() == "annotation") { // Update displayed notes for annotation elements
        if (json["Notes"].dump().compare(m_annotation["Notes"]) != 0) {
            setLabelText(QString::fromStdString(json["Notes"]));
        }
    }
    m_annotation = json;
    m_annotationCache = QString::fromStdString(json.dump()).toUtf8();
    setItemStateFlag(IS_Attribute_Changed);
}

QString BGElement::getConstiutiveRelation()
{
    if (m_annotation.contains("constitutive_relations"))
        return QString::fromStdString(m_annotation["constitutive_relations"]);
    return "";
}

QPixmap BGElement::getRenderedLatex(int font_size)
{
    // create a JKQTMathText object.
    JKQTMathText mathText;

    // configure its properties to influence the rendering (e.g. fonts to use,
    // font size, ...)
    mathText.useXITS();
    // QFont font = getAttribute("label.font").value<QFont>();
    // QColor colour = getAttribute("label.color").value<QColor>();
    // JKQTMathText uses its own fonts etc
    mathText.setFontSize(font_size);
    if (itemColours.contains(m_type)) {
        mathText.setFontColor(itemColours[m_type]);
    }
    // parse some LaTeX code (the Schroedinger's equation)
    mathText.parse(mDisplayName);

    // use the draw() methods to draw the equation using a QPainter (here onto a
    // QPixmap)
    QPainter painter;
    QSizeF lsize = mathText.getSize(painter);
    auto width = qMax(lsize.width(), lsize.height()) + 5;
    auto height = qMax(lsize.width(), lsize.height()) + 5;
    if (width < minDim)
        width = minDim - 5;
    if (height < minDim)
        height = minDim - 5;

    QPixmap pix(width, height);
    pix.fill(Qt::transparent);
    painter.begin(&pix);
    mathText.draw(painter, Qt::AlignCenter,
                  QRectF(-5, 0, pix.width(), pix.height()), false);
    painter.end();
    return pix;
}
// ports

BGPort *BGElement::addPort(const QByteArray &port_id, QPointF pos)
{
    if (getClassType() == "annotation")
        return nullptr;

    if (m_ports.contains(port_id))
        return nullptr;

    QByteArray newPortId(port_id);
    if (port_id.isEmpty()) {
        int suffix = 1;
        if (getMaxPorts() > 2)
            suffix = 2; // For junctions start at 2 as port 1 cannot be deleted
        do {
            newPortId = "Port " + QByteArray::number(suffix++);
        } while (m_ports.contains(newPortId));
    }
    QPointF ppos = pos;
    if (pos == QPointF(-1000, -1000)) {
        ppos.setX(0);
        ppos.setY(minDim / 2);
    }
    BGPort *port = new BGPort(this, newPortId, ppos);
    m_ports[newPortId] = port;

    updateCachedItems();

    return port;
}

bool BGElement::removePort(const QByteArray &port_id)
{
    if (port_id.isEmpty() || !m_ports.contains(port_id))
        return false;

    BGPort *port = m_ports.take(port_id);
    if (!port->getDeleteFlag()) {
        return false;
    }
    // to do. update edges.

    delete port;

    updateCachedItems();

    return true;
}

void BGElement::reorientPort(int dir)
{
    if (maxPorts < 3) // Ignore
    {
        bool horiz = dir == Qt::Key_Right;
        // bool vert = dir == Qt::Key_Up;
        auto ks = m_ports.begin();
        int prevAlign = -1; // Capture the alignment of previous
        {
            auto pt = ks.value();
            auto align = pt->getAlign();
            if (align == -1) {
                if (horiz) {
                    pt->setAlignment(Qt::AlignLeft);
                    prevAlign = Qt::AlignLeft;
                } else {
                    pt->setAlignment(Qt::AlignTop);
                    prevAlign = Qt::AlignTop;
                }
            } else {
                if (horiz) {
                    if (align & Qt::AlignLeft) {
                        pt->setAlignment(Qt::AlignRight);
                        prevAlign = Qt::AlignRight;
                    } else {
                        pt->setAlignment(Qt::AlignLeft);
                        prevAlign = Qt::AlignLeft;
                    }
                } else {
                    if (align & Qt::AlignTop) {
                        pt->setAlignment(Qt::AlignBottom);
                        prevAlign = Qt::AlignBottom;
                    } else {
                        pt->setAlignment(Qt::AlignTop);
                        prevAlign = Qt::AlignTop;
                    }
                }
            }
        }
        if (m_ports.size() > 1) {
            ks++;
            auto pt = ks.value();
            if (horiz)
                if (prevAlign == Qt::AlignLeft)
                    pt->setAlignment(Qt::AlignRight);
                else
                    pt->setAlignment(Qt::AlignLeft);
            else if (prevAlign == Qt::AlignTop)
                pt->setAlignment(Qt::AlignBottom);
            else
                pt->setAlignment(Qt::AlignTop);
        }
        updatePortsLayout();
    }
}

bool BGElement::movePort(const QByteArray &port_id, QPointF pos)
{
    if (!m_ports.contains(port_id))
        return false;

    BGPort *port = m_ports[port_id];
    port->setOffset(pos);

    updatePortsLayout();

    return true;
}

bool BGElement::renamePort(const QByteArray &port_id, const QByteArray &new_id)
{
    if (!m_ports.contains(port_id))
        return false;

    if (port_id == new_id)
        return true;

    if (!m_ports.contains(new_id)) {
        BGPort *port = m_ports[port_id];
        m_ports[new_id] = port;
        m_ports.remove(port_id);

        port->setId(new_id);

        updateCachedItems();

        return true;
    }

    // port exists: bail out for now...
    return false;
}

BGPort *BGElement::getPort(const QByteArray &port_id) const
{
    if (port_id.isEmpty() || !m_ports.contains(port_id))
        return nullptr;
    else
        return m_ports[port_id];
}

QByteArrayList BGElement::getPortIds() const
{
    return m_ports.keys();
}

// serialization

bool BGElement::storeTo(QDataStream &out, quint64 version64) const
{
    out << maxPorts;

    out << getSize();

    out << pos() << itemFlags();

    out << zValue();

    out << m_icon;

    out << m_type;
    // out << QString::fromStdString(m_definition.dump());

    out << mDisplayName;

    out << m_proxy;

    int portsCount = m_ports.size();
    out << portsCount;

    out << m_annotationCache;

    for (auto port : m_ports)
        port->storeTo(out, version64);

    return SceneItem::storeTo(out, version64);
}

bool BGElement::restoreFrom(QDataStream &out, quint64 version64)
{
    out >> maxPorts;

    QSizeF size;
    out >> size;
    resize(size);

    static QPointF p;
    out >> p;

    static int f;
    out >> f;
    // setItemFlags(f);

    qreal z;
    out >> z;

    out >> m_icon;

    out >> m_type;
    // QString jsonDef;
    // out >> jsonDef;
    // m_definition = nlohmann::json::parse(jsonDef.toStdString());
    out >> mDisplayName;

    out >> m_proxy;
    // ports
    m_ports.clear();

    int count = 0;
    out >> count;
    QByteArray qarr;
    out >> qarr;
    if (m_annotationCache.compare(qarr) != 0) {
        m_annotationCache = qarr;
        m_annotation = nlohmann::json::parse(m_annotationCache.toStdString());
    }

    QByteArray id;
    QPointF pos;
    int align, pDirection;
    bool canDelete;
    for (int i = 0; i < count; ++i) {
        out >> id;
        out >> pos;
        out >> align;
        out >> canDelete;
        out >> pDirection;

        BGPort *port = new BGPort(this, id, pos);
        port->setAlignment(align);
        port->setDeleteFlag(canDelete);
        port->setDirection(pDirection);
        m_ports[id] = port;

        QBrush br;
        out >> br;
        port->setBrush(br);
        QPen pn;
        out >> pn;
        port->setPen(pn);
        QRectF r;
        out >> r;
        port->setRect(r);
        // update
        port->onParentGeometryChanged();
    }

    setPos(p);
    setZValue(z);

    return SceneItem::restoreFrom(out, version64);
}

bool BGElement::allowStartConnection() const
{
    // For junction types this should always be true
    if (maxPorts < 3)
        return m_connections.size() < m_ports.size();
    else
        return true;
}

// element operations

bool BGElement::merge(BGElement *element, const QByteArray &port_id)
{
    if (!element || (element == this))
        return false;

    bool allowConnect =
        allowStartConnection(); // Only allow junctions to create new ports
    bool allowCircled = allowCircledConnection();
    if (port_id.size() == 0)
        allowConnect = false;
    // Check if port is free
    for (auto conn : m_connections) {
        if ((conn->firstPortId() == port_id && conn->firstElement() == element) || (conn->lastPortId() == port_id && conn->lastElement() == element)) {
            allowConnect = false;
            break;
        }
    }
    if (!allowConnect) {
        // kill old element
        delete element; // will be removed by removed connections
        return true;
    }

    // make a copy because element's connections list will be updated
    QSet<BGConnection *> toReconnect = element->m_connections;

    for (BGConnection *conn : toReconnect) {
        conn->reattach(element, this, port_id);
        conn->onParentGeometryChanged();
    }

    // kill old element
    if (element->getClassType() != "annotation")
        delete element; // will be removed by removed connections
    else {              // For annotation elements set the zValue
        // set the z pos to below the elements
        element->setZValue(zValue() - 1);
    }

    if (!allowCircled) {
        toReconnect = m_connections;

        for (BGConnection *conn : toReconnect) {
            if (conn->isCircled())
                delete conn;
        }
    }

    return true;
}

QList<BGElement *> BGElement::unlink()
{
    QList<BGElement *> nodes;

    float xpos = x();
    float ypos = y();

    float xstep = getScene() ? getScene()->getGridSize() : 10;

    while (m_connections.size() >= 2) {
        BGConnection *c = *m_connections.begin();

        BGElement *n = dynamic_cast<BGElement *>(clone());
        xpos += xstep;
        n->setPos(xpos, ypos);

        c->reattach(this, n);

        nodes.append(n);
    }

    return nodes;
}

QList<BGElement *> BGElement::getCollidingElements() const
{
    QList<BGElement *> nodes;

    BGEditorScene *scene = getScene();
    if (!scene)
        return nodes;

    QList<QGraphicsItem *> itemsUnderMouse = scene->items(pos());

    for (auto i = 0; i < itemsUnderMouse.size(); ++i) {
        BGElement *element = dynamic_cast<BGElement *>(itemsUnderMouse.at(i));
        if (!element || element == this)
            continue;

        nodes.append(element);
    }

    return nodes;
}

QSet<BGConnection *> BGElement::getInConnections() const
{
    QSet<BGConnection *> edges;

    for (auto edge : m_connections) {
        if (edge->lastElement() == this)
            edges << edge;
    }

    return edges;
}

QSet<BGConnection *> BGElement::getOutConnections() const
{
    QSet<BGConnection *> edges;

    for (auto edge : m_connections) {
        if (edge->firstElement() == this)
            edges << edge;
    }

    return edges;
}

void BGElement::pauseCacheUpdates()
{
    m_canUpdateCache = false;
}

void BGElement::restartCacheUpdates()
{
    m_canUpdateCache = true;
}

QPointF BGElement::getIntersectionPoint(const QLineF &line,
                                        const QByteArray &port_id,
                                        const double shadow) const
{
    // port
    if (port_id.size()) {
        if (BGPort *port = getPort(port_id)) {
            double shift = (port->boundingRect().width() / 2) * shadow;
            auto angle = qDegreesToRadians(line.angle());
            return port->scenePos() + QPointF(shift * qCos(angle), -shift * qSin(angle));
        }
    }

    // circular shape
    if (m_shapeCache.isEmpty()) {
        // circle/ring
        if (rect().height() == rect().width()) {
            auto shift = shadow * rect().width() / 2;
            auto angle = qDegreesToRadians(line.angle());
            return pos() + QPointF(shift * qCos(angle), -shift * qSin(angle));
        }

        // ellipse
        {
            QRectF r(rect());
            r.moveCenter(pos());
            QPolygonF p(r);
            return Utils::closestIntersection(line, p);
        }
    }

    // polygon
    QPolygonF scenePolygon = m_shapeCache.translated(pos());
    return Utils::closestIntersection(line, scenePolygon);
}

///
/// \brief BGElement::onConnectionAttach
/// \param conn
///
void BGElement::onConnectionAttach(BGConnection *conn)
{
    Q_ASSERT(conn != nullptr);
    if (m_proxy != -1) {
        m_proxy = 1;
    }
    m_connections.insert(conn);

    updateConnections();
}

///
/// \brief BGElement::onConnectionDetach
/// \param conn
///
void BGElement::onConnectionDetach(BGConnection *conn)
{
    Q_ASSERT(conn != nullptr);

    // If I am junction then remove port related to the connection
    // Always have one port around
    if (maxPorts > 2) {
        if (m_ports.contains(conn->firstPortId()) && conn->firstElement() == this) {
            removePort(conn->firstPortId());
        }
        if (m_ports.contains(conn->lastPortId()) && conn->lastElement() == this) {
            removePort(conn->lastPortId());
        }
        if (m_connections.contains(conn))
            m_connections.remove(conn);
    } else {
        m_connections.remove(conn);

        updateConnections();
    }
}

void BGElement::updateConnections()
{
    // optimize: no update while restoring
    if (s_duringRestore)
        return;
}

void BGElement::onConnectionDeleted(BGConnection *conn)
{
    onConnectionDetach(conn);

    // remove orphan element if allowed
    if (m_connections.isEmpty() && !(m_Flags & NF_OrphanAllowed))
        delete this;
}

void BGElement::onPortDeleted(BGPort *port)
{
    if (m_ports.size() < 1) // When the element is references across multiple entities like
                            // connections, etc. this check is required to handle memory issues
        return;

    for (auto edge : m_connections) {
        edge->onElementPortDeleted(this, port->getId());
    }
    m_ports.remove(port->getId());
}

void BGElement::onPortRenamed(BGPort *port, const QByteArray &old_id)
{
    for (auto edge : m_connections) {
        edge->onElementPortRenamed(this, port->getId(), old_id);
    }
}

void BGElement::onItemMoved(const QPointF & /*delta*/)
{
    for (BGConnection *conn : m_connections) {
        conn->onElementMoved(this);
    }
}

void BGElement::onItemRestored()
{
    updateCachedItems();

    updateConnections();
}

void BGElement::onDroppedOn(
    const QSet<InteractiveItemInterface *> &accepted_items,
    const QSet<InteractiveItemInterface *> & /*rejectedItems*/)
{
    if (accepted_items.size()) {
        // check for ports
        for (auto item : accepted_items) {
            BGPort *port = dynamic_cast<BGPort *>(item);
            if (port) {
                BGElement *element = port->getElement();
                // IF it is an annotation element change the z value
                if (getClassType() == "annotation") {
                    setZValue(element->zValue() - 1);
                    return;
                }
                if (element->junctionType()) {
                    // Check if the source element is not already connected
                    bool connect = true;
                    for (auto &c : m_connections) {
                        auto connections = c->firstElement()->m_connections;
                        for (auto &x : connections) {
                            if (x->firstElement() == element || x->lastElement() == element) {
                                connect = false;
                                break;
                            }
                        }
                        break;
                    }
                    if (!connect) {
                        delete this;
                        return;
                    }
                    if (element->getMaxPorts() > 2) {
                        // Dropped on a junction
                        // Calculate position, go through connections and find the other
                        // Host element (this) is connectHelp
                        for (auto &c : m_connections) {
                            auto fElem = c->firstElement();
                            QPointF point = fElem->scenePos();
                            QPointF target = element->scenePos();
                            QLineF line(point, target);
                            // auto vec = (target - point) / line.length();
                            // Check if the other element is a junction as well
                            // If so add a new port on that side and connect to it
                            if (fElem->getMaxPorts() > 2 && fElem->junctionType()) {
                                auto oldPort = c->firstPortId();
                                auto newPort = fElem->addPort();
                                newPort->setAlignment(-1);
                                c->reattach(fElem, oldPort, newPort->getId());
                                // Positions are calculated by the Connection
                            }

                            auto newPort = element->addPort();
                            newPort->setAlignment(-1);
                            element->merge(this, newPort->getId());
                            element->setSelected(true);
                            element->removeOrphanPorts();
                            return;
                        }
                    } else { // Handle transformers and gyrators
                        // Check if the dropped element is a junction as well
                        // If so add a new port on that side and connect to it
                        for (auto &c : m_connections) {
                            auto fElem = c->firstElement();
                            // QPointF point = fElem->pos();
                            if (fElem->getMaxPorts() > 2 && fElem->junctionType()) {
                                auto oldPort = c->firstPortId();
                                auto newPort = fElem->addPort();
                                newPort->setAlignment(
                                    -1); // Setting invalid value for distributePorts to detect
                                c->reattach(fElem, oldPort, newPort->getId());
                                // Calculate port position
                                // fElem->distributePorts();
                                break;
                            }
                        }
                        if (port->getDirection() == 1) { // If the port is out going then do not accept
                            delete this;
                            return;
                        }

                        element->merge(this, port->getId());
                        element->setSelected(true);
                        element->removeOrphanPorts();
                        element->updatePortsLayout();

                        return;
                    }
                } else {
                    // I am not a junction
                    // Check if I am a reaction and handle io
                    if (element->getMaxPorts() == 2) {
                        if (port->getDirection() == 1) { // If the port is out going then do not accept
                            delete this;
                            return;
                        }
                    }

                    for (auto &con : m_connections) {
                        // Get the correct connection
                        if (con->lastElement() != this)
                            continue;
                        auto fElem = con->firstElement();
                        // Check if the source item is a junction
                        // if so create a new port for that item and reattach connection
                        if (fElem->getMaxPorts() > 2 && fElem->junctionType()) {
                            auto oldPort = con->firstPortId();
                            auto newPort = fElem->addPort();
                            newPort->setAlignment(
                                -1); // Setting invalid value for distributePorts to detect
                            con->reattach(fElem, oldPort, newPort->getId());
                            // Calculate port position
                            con->onParentGeometryChanged();
                            break;
                        }
                        // Handle reactions
                        if (fElem->getMaxPorts() == 2 && fElem->getClassType() == "passive") {
                            // TODO check dimensions
                            break;
                        } else if (fElem->junctionType()) {
                            break;
                        } else {
                            // Do not let elements connect to one another except for
                            // annotations and reactions
                            if (element->getClassType() != "annotation" && element->getMaxPorts() == 1) {
                                delete this; // Ensure snap action is not carried on
                                return;
                            }
                            break;
                        }
                    }
                    // Annotations do not have connections so handle them here too
                    if (element->getClassType() == "annotation") {
                        delete this; // Ensure snap action is not carried on
                        return;
                    }
                    // TODO Check dimension match
                    element->merge(this, port->getId());
                    element->setSelected(true);
                    element->removeOrphanPorts();
                    element->updatePortsLayout();
                }
                return;
            }
        }
    }
}

ItemDragTestResult BGElement::acceptDragFromItem(QGraphicsItem *dragged_item)
{
    if (dynamic_cast<BGElement *>(dragged_item))
        return Accepted;
    else
        return Ignored;
}

void BGElement::removeOrphanPorts()
{
    // Only for 0,1 junctions
    if (maxPorts > 2) {
        std::vector<BGPort *> delPorts;
        std::vector<QByteArray> keys;

        for (auto it = m_ports.begin(); it != m_ports.end(); it++) {
            if (it.value()->getConnection() == nullptr && it.value()->getDeleteFlag()) {
                delPorts.push_back(it.value());
                keys.push_back(it.key());
            }
        }
        for (auto c : keys) {
            m_ports.remove(c);
        }
        for (auto c : delPorts) {
            delete c;
        }
    }
}

// override

QVariant BGElement::itemChange(QGraphicsItem::GraphicsItemChange change,
                               const QVariant &value)
{
    if (change == ItemSceneHasChanged) {
        // set default ID
        setDefaultId();

        // update attributes cache after attach to scene
        updateCachedItems();

        return value;
    }
    if (m_id != "ConnectHelp" && change == ItemPositionChange) { // handle position of auto ports
        for (auto con : m_connections) {
            if (con->firstElement() == this) {
                if (m_ports[con->firstPortId()]->getAlign() == -1) {
                    BGPort *port = m_ports[con->firstPortId()];
                    BGPort *lport = con->lastElement()->getPort(con->lastPortId());
                    QLineF line(port->scenePos(), lport->scenePos());
                    double shift = (port->boundingRect().width() / 2) * 1.5;
                    auto angle = qDegreesToRadians(line.angle());
                    port->moveBy(shift * qCos(angle), -shift * qSin(angle));
                    if (lport->getAlign() == -1) {
                        shift = (lport->boundingRect().width() / 2) * 1.5;
                        // angle = qDegreesToRadians(3.141+line.angle());
                        lport->moveBy(shift * qCos(3.141 + angle),
                                      -shift * qSin(3.141 + angle));
                    }
                }
            }
            if (con->lastElement() == this) {
                if (m_ports[con->lastPortId()]->getAlign() == -1) {
                    BGPort *port = m_ports[con->lastPortId()];
                    BGPort *lport = con->firstElement()->getPort(con->firstPortId());
                    QLineF line(port->scenePos(), lport->scenePos());
                    double shift = (port->boundingRect().width() / 2) * 1.5;
                    auto angle = qDegreesToRadians(line.angle());
                    port->moveBy(shift * qCos(angle), -shift * qSin(angle));
                    if (lport->getAlign() == -1) {
                        shift = (lport->boundingRect().width() / 2) * 1.5;
                        // angle = qDegreesToRadians(3.141+line.angle());
                        lport->moveBy(shift * qCos(3.141 + angle),
                                      -shift * qSin(3.141 + angle));
                    }
                }
            }
        }
    }

    if (change == ItemPositionHasChanged) {
        setItemStateFlag(IS_Attribute_Changed);

        QPointF d = value.toPointF() - scenePos();
        onItemMoved(d);

        return value;
    }

    if (change == ItemSelectedHasChanged) {
        onItemSelected(value.toBool());

        return value;
    }

    return value;
}

void BGElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *)
{
    bool isSelected = (option->state & QStyle::State_Selected);

    painter->setClipRect(boundingRect());

    // get color (to optimize!)
    QColor color = getAttribute(QByteArrayLiteral("color")).value<QColor>();
    if (color.isValid())
        painter->setBrush(color);
    else
        painter->setBrush(Qt::NoBrush);

    qreal strokeSize = getAttribute(QByteArrayLiteral("stroke.size")).toDouble();
    strokeSize = qMax(0.1, strokeSize);

    QColor strokeColor =
        getAttribute(QByteArrayLiteral("stroke.color")).value<QColor>();

    int strokeStyle = Utils::textToPenStyle(
        getAttribute(QByteArrayLiteral("stroke.style")).toString(),
        Qt::SolidLine);
    QRectF r = Shape::boundingRect();

    // selection background outline
    if (isSelected) {
        painter->setPen(QPen(Qt::darkCyan, strokeSize + 5, Qt::SolidLine,
                             Qt::FlatCap, Qt::RoundJoin));
        painter->setOpacity(0.3);

        // draw shape: disc if no cache
        if (m_shapeCache.isEmpty()) {
            painter->drawEllipse(r);
        } else {
            painter->drawPolygon(m_shapeCache);
        }
        if (!m_icon.isNull()) {
            painter->drawPixmap(iconTop, iconLeft, m_iconCache);
        }
    }

    // hover opacity
    if (itemStateFlags() & IS_Drag_Accepted)
        painter->setOpacity(0.6);
    else
        painter->setOpacity(1.0);

    painter->setPen(QPen(strokeColor, strokeSize, (Qt::PenStyle)strokeStyle));

    // draw shape: disc if no cache
    if (m_shapeCache.isEmpty()) {
        painter->drawEllipse(r);
    } else {
        painter->drawPolygon(m_shapeCache);
    }
    if (!m_icon.isNull()) {
        painter->drawPixmap(iconTop, iconLeft, m_iconCache);
    }
}

QRectF BGElement::boundingRect() const
{
    QRectF r = Shape::boundingRect();

    // in case of bold selection
    if (auto scene = getScene()) {
        const int margin = scene->getBoundingMargin();
        return r.adjusted(-margin, -margin, margin, margin);
    } else
        return r;
}

void BGElement::updateCachedItems()
{
    if (!m_canUpdateCache)
        return;
    SceneItem::updateCachedItems();

    auto shapeCache = m_shapeCache;
    auto sizeCache = m_sizeCache;

    recalculateShape();

    // update caches & connections
    if (m_shapeCache != shapeCache || m_sizeCache != sizeCache) {
        // update icon, ports & edges
        updatePortsLayout();

        if (!m_icon.isNull()) {
            const QRectF r = Shape::boundingRect();
            // const QSizeF mm = m_icon.size();
            // QRectF
            // r(rs.x(),rs.y(),qMax(rs.width(),mm.width()),qMax(rs.height(),mm.height()));
            m_iconCache =
                m_icon.scaled(QSize(r.width(), r.height()), Qt::KeepAspectRatio);

            const QSize ms = m_iconCache.size();
            iconTop = r.topLeft().x() + (r.height() - ms.height()) / 2;
            iconLeft = r.topLeft().y() + (r.width() - ms.width()) / 2;
        }
    }

    update();
}

void BGElement::updatePortsLayout()
{
    prepareGeometryChange();
    // update ports layout
    for (auto port : m_ports.values()) {
        port->onParentGeometryChanged();
    }

    // update edges as well
    for (auto edge : m_connections) {
        edge->onParentGeometryChanged();
    }
}

void BGElement::updateLabelPosition()
{
    int w = m_labelItem->boundingRect().width();
    int h = m_labelItem->boundingRect().height();

    // QRectF r = Shape::boundingRect();

    if (getClassType() != "annotation") {
        m_labelItem->setPos(-w / 2, boundingRect().height() / 2 + h / 3); // put label at the bottom
    } else {
        QRectF myRect = boundingRect();
        QSizeF sz = getAttribute("size").toSizeF();
        bool extentChanged = false;
        if (myRect.width() < w) {
            sz.setWidth(w * 1.2);
            extentChanged = true;
        }
        if (myRect.height() < h) {
            sz.setHeight(h * 1.2);
            extentChanged = true;
        }
        if (extentChanged) {
            auto top = myRect.top();
            auto left = myRect.left();
            // Not calling setAttribute as that will lead to recursive calls
            SceneItem::setAttribute("size", sz);
            recalculateShape();
            // Get the new bounding rect and move back to the previous top left
            // position
            auto nyRect = boundingRect();
            auto ntop = nyRect.top();
            auto nleft = nyRect.left();
            moveBy(left - nleft, top - ntop);
            update();
        }

        m_labelItem->setPos(-boundingRect().width() / 2.5,
                            -boundingRect().height() / 2.5); // put label at top left
    }
}

// priv

void BGElement::recalculateShape()
{
    QSizeF sz = getAttribute("size").toSizeF();
    const QSizeF mm = m_icon.size();

    if (sz.width() < mm.width())
        sz.setWidth(mm.width());
    if (sz.height() < mm.height())
        sz.setHeight(mm.height());

    resize(sz);

    QRectF r = Shape::boundingRect();

    m_shapeCache.clear();
    m_sizeCache = r;

    QByteArray shapeType = getAttribute("shape").toByteArray();
    if (shapeType == "square") {
        m_shapeCache = r;
    } else if (shapeType == "diamond") {
        float rx = r.center().x();
        float ry = r.center().y();

        m_shapeCache << QPointF(rx, ry - r.height() / 2)
                     << QPointF(rx + r.width() / 2, ry)
                     << QPointF(rx, ry + r.height() / 2)
                     << QPointF(rx - r.width() / 2, ry)
                     << QPointF(rx, ry - r.height() / 2);
    } else if (shapeType == "hexagon") {
        float rx = r.center().x();
        float ry = r.center().y();

        m_shapeCache << QPointF(r.left() + r.width() / 3, ry - r.height() / 2)
                     << QPointF(r.left() + r.width() / 3 * 2, ry - r.height() / 2)
                     << QPointF(rx + r.width() / 2, ry)
                     << QPointF(r.left() + r.width() / 3 * 2, ry + r.height() / 2)
                     << QPointF(r.left() + r.width() / 3, ry + r.height() / 2)
                     << QPointF(r.left(), ry)
                     << QPointF(r.left() + r.width() / 3, ry - r.height() / 2);
    } else if (shapeType == "triangle") {
        m_shapeCache << r.bottomLeft() << r.bottomRight()
                     << QPointF(r.topRight() + r.topLeft()) / 2 << r.bottomLeft();
    } else if (shapeType == "triangle2") {
        m_shapeCache << r.topLeft() << r.topRight()
                     << QPointF(r.bottomRight() + r.bottomLeft()) / 2
                     << r.topLeft();
    } else // "disc"
    {
        // no cache
    }
}

// events

void BGElement::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_internalStateFlags |= IS_Hover;

    onHoverEnter(this, event);

    update();
}

void BGElement::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_internalStateFlags &= ~IS_Hover;

    onHoverLeave(this, event);

    update();
}

void BGElement::resize(float size)
{
    setRect(-size / 2, -size / 2, size, size);
}

void BGElement::resize(float w, float h)
{
    setRect(-w / 2, -h / 2, w, h);
}

void BGElement::resize(const QSizeF &size)
{
    resize(size.width(), size.height());
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR