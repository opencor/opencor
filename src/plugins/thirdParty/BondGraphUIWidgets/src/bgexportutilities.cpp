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
#include "bgexportutilities.h"

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QSettings>
#include <QtXml/QDomDocument>

#include <string>

#include "bgconnection.h"
#include "bgdconnection.h"
#include "bgeditorscene.h"
#include "bgelement.h"
#include "bgport.h"
#include "sceneitem.h"
#include "utils.h"
#include "thirdparty/widgetbox/widgetbox.h"

#include "bgprojectdetails.h"
#include "bondgraph.hpp"

//#define DEBUG_CELLML_EXPORT

namespace OpenCOR {
namespace BondGraphUI {

const quint64 version64 = 12; // should match the one in bgeditorscene.cpp

std::string serializeToQString(const QVariant &v) {
  QByteArray byteArray;
  QBuffer writeBuffer(&byteArray);
  writeBuffer.open(QIODevice::WriteOnly);
  QDataStream out(&writeBuffer);

  out << v;

  writeBuffer.close();

  return QString(byteArray.toBase64()).toStdString();
}

QVariant deSerializeFromQString(const std::string &str) {
  QByteArray readArr = QByteArray::fromBase64(str.c_str());
  QBuffer readBuffer(&readArr);
  readBuffer.open(QIODevice::ReadOnly);
  QDataStream in(&readBuffer);

  QVariant var;

  in >> var;

  return var;
}

void to_json(nlohmann::json &j, const QByteArray &p) {
  j = std::string(p.constData(), p.length());
}

void from_json(const nlohmann::json &j, QByteArray &p) {
  std::string stdString = j;
  p = QByteArray(stdString.c_str(), (int)stdString.length());
}

void to_json(nlohmann::json &j, const BGEditorScene &p) {
  j["versionId"] = version64;
  j["counter"] = SceneItem::getItemCount();
  // items
  QMap<SceneItem *, uint> sortedMap;

  QList<QGraphicsItem *> allItems = p.items();

  for (QGraphicsItem *item : allItems) {
    SceneItem *citem = dynamic_cast<SceneItem *>(item);
    if (citem) {
      sortedMap[citem] = quint64(citem);
    }
  }
  nlohmann::json jSceneItems;
  unsigned int order = 0;
  for (SceneItem *citem : sortedMap.keys()) {
    nlohmann::json sJson;
    sJson["typeId"] = citem->typeId();
    if (BGElement *bitem = dynamic_cast<BGElement *>(citem)) {
      sJson["definition"] = *bitem;
    } else if (BGPort *pitem = dynamic_cast<BGPort *>(citem)) {
      sJson["definition"] = *pitem;
    } else if (BGDConnection *nitem = dynamic_cast<BGDConnection *>(citem)) {
      sJson["definition"] = *nitem;
    } else {
      sJson["definition"] = *citem;
    }
    sJson["order"] = order++;
    jSceneItems[citem->getId().toStdString()] = sJson;
  }
  j["sceneItems"] = jSceneItems;

  //   // attributes

  //   nlohmann::json attributes;
  //   attributes["version"] = (quint64)0x12345678;
  //   attributes["size"] = p.m_classAttributes.size();

  //   for (auto classAttrsIt = p.m_classAttributes.constBegin();
  //        classAttrsIt != p.m_classAttributes.constEnd(); ++classAttrsIt) {
  //     nlohmann::json avals = nlohmann::json::array();
  //     for (const auto &attr : classAttrsIt.value()) {
  //         nlohmann::json atv;
  //         atv["id"] = attr.id;
  //         atv["name"] = attr.name.toStdString();
  //         //Dirty hack
  //         atv["defaultValue"] = serializeToQString(attr.defaultValue);
  //         atv["valueType"] = attr.valueType;
  //         avals.push_back(atv);
  //     }
  //     auto key = classAttrsIt.key();
  //     attributes[serializeToQString(key)] = avals;
  //   }

  //   nlohmann::json idmap;
  //   for(const auto& im : p.m_classToSuperIds.keys())
  //   {
  //       auto key = serializeToQString(im);
  //       idmap[key] = p.m_classToSuperIds[im];
  //   }
  //   j["classToSuperIds"]=idmap;

  //   // visible attributes
  //   nlohmann::json visMap;
  //   for(const auto& im : p.m_classAttributesVis.keys())
  //   {
  //       auto arr = p.m_classAttributesVis[im]; //QSet<QByteArray>
  //       nlohmann::json vis = nlohmann::json::array();
  //       for(const auto &v : arr){
  //           vis.push_back(std::string(v.constData(), v.length()));
  //       }
  //       auto key = serializeToQString(im);
  //       idmap[key] = p.m_classToSuperIds[im];
  //   }
  //   j["visibleAttributes"] = visMap;
  //   j["attributes"] = attributes;
}

void from_json(const nlohmann::json &j, BGEditorScene &p) {
  p.initialize();
  // version
  // quint64 storedVersion =  j["versionId"];
  nlohmann::json jSceneItems = j["sceneItems"];
  if (j.contains("counter")) {
    SceneItem::setItemCount(j["counter"]);
  } else {
    // Temporary fix
    SceneItem::setItemCount((int)jSceneItems.size() + 1);
  }

  BGElement *elementsFactory = p.factory<BGElement>();
  BGConnection *connectionsFactory = p.factory<BGDConnection>();
  QMap<QString, BGElement *> elements;
  // First create the elements
  for (const auto &js : jSceneItems.items()) {
    QString iid = QString::fromStdString(js.key());
    const nlohmann::json &sJson = js.value();
    std::string type = sJson["typeId"];
    if (type == "BGElement") {
      auto element = dynamic_cast<BGElement *>(elementsFactory->create());
      const nlohmann::json &definition = sJson["definition"];
      const nlohmann::json &position = definition["position"];
      QPoint pos(position["x"], position["y"]);
      p.addItem(element);
      element->setPos(pos);
      // Do this after adding the element to the scene
      from_json(definition, *element);
      if (element->getClassType() == "annotation") {
        element->showLabel(true);
      }
      elements[element->getId()] = element;
    } else if (type == "BGDConnection") {
      continue;
    } else {
      throw "JSON restore not implemented for type " + type;
    }
  }
  // Now create the connections
  for (const auto &js : jSceneItems.items()) {
    QString iid = QString::fromStdString(js.key());
    const nlohmann::json &sJson = js.value();
    std::string type = sJson["typeId"];
    if (type == "BGDConnection") {
      auto connection =
          dynamic_cast<BGDConnection *>(connectionsFactory->create());
      const nlohmann::json &definition = sJson["definition"];
      from_json(definition, *connection);
      QString p1 = QString::fromStdString(definition["firstPort"]);
      QString p2 = QString::fromStdString(definition["secondPort"]);
      if (elements[QString::fromStdString(definition["first"])] != nullptr) {
        connection->setFirstElement(
            elements[QString::fromStdString(definition["first"])], p1.toUtf8());
        if (elements[QString::fromStdString(definition["first"])]->getPort(
                p1.toUtf8()) != nullptr)
          elements[QString::fromStdString(definition["first"])]
              ->getPort(p1.toUtf8())
              ->setConnection(connection);
        else {
          if (elements[QString::fromStdString(definition["first"])]
                  ->getMaxPorts() > 2) {
            /*
             *  Some times ports are not written to the json file, in such cases
             * create a port for junctions Sometimes Port 1 of junction is
             * connected, so handle that too
             */
            if (p1 != "Port 1")
              elements[QString::fromStdString(definition["first"])]->addPort(
                  p1.toUtf8());
            else {
              BGPort *np = elements[QString::fromStdString(definition["first"])]
                               ->addPort();
              p1 = np->getId();
            }
            elements[QString::fromStdString(definition["first"])]
                ->getPort(p1.toUtf8())
                ->setConnection(connection);
          } else {
            throw std::runtime_error("File is corrupt! Port " +
                                     p1.toStdString() + " does not exist!");
          }
        }
      }
      if (elements[QString::fromStdString(definition["second"])]) {
        connection->setLastElement(
            elements[QString::fromStdString(definition["second"])],
            p2.toUtf8());
        if (elements[QString::fromStdString(definition["second"])]->getPort(
                p2.toUtf8()))
          elements[QString::fromStdString(definition["second"])]
              ->getPort(p2.toUtf8())
              ->setConnection(connection);
        else {
          if (elements[QString::fromStdString(definition["second"])]
                  ->getMaxPorts() > 2) {
            if (p2 != "Port 1")
              elements[QString::fromStdString(definition["second"])]->addPort(
                  p2.toUtf8());
            else {
              BGPort *np =
                  elements[QString::fromStdString(definition["second"])]
                      ->addPort();
              p2 = np->getId();
            }
            elements[QString::fromStdString(definition["second"])]
                ->getPort(p2.toUtf8())
                ->setConnection(connection);
          } else {
            throw std::runtime_error("File is corrupt! Port " +
                                     p2.toStdString() + " does not exist!");
          }
        }
      }
      p.addItem(connection);
    }
  }

  for (auto it = elements.begin(); it != elements.end(); it++) {
    it.value()->removeOrphanPorts();
  }
}

void to_json(nlohmann::json &res, const BGDConnection &p) {
  res["type"] = "BGDConnection";
  res["first"] = p.m_firstElement->getId().toStdString();
  res["firstPort"] = p.m_firstPortId;
  res["second"] = p.m_lastElement->getId().toStdString();
  res["secondPort"] = p.m_lastPortId;
  res["bendFactor"] = p.m_bendFactor;
  res["controlPos"] = {{"x", p.m_controlPos.x()}, {"y", p.m_controlPos.y()}};
  res["controlPoint"] = {{"x", p.m_controlPoint.x()},
                         {"y", p.m_controlPoint.y()}};
  const SceneItem &bgi = dynamic_cast<const SceneItem &>(p);
  res["sceneitem"] = bgi;
}

void from_json(const nlohmann::json &j, BGDConnection &p) {
  nlohmann::json cp = j["controlPoint"];
  nlohmann::json cps = j["controlPos"];
  QPointF controlPoint(cp["x"], cp["y"]);
  QPointF controlPos(cps["x"], cps["y"]);
  p.m_controlPoint = controlPoint;
  p.m_controlPos = controlPos;
  SceneItem &bgi = dynamic_cast<SceneItem &>(p);
  from_json(j["sceneitem"], bgi);
  p.updateCachedItems();
}

void to_json(nlohmann::json &j, const BGElement &p) {
  j["mid"] = p.getId().toStdString();
  j["maxPorts"] = p.maxPorts;
  const auto size = p.getSize();
  j["size"] = {{"width", size.width()}, {"height", size.height()}};
  const auto position = p.pos();
  j["position"] = {{"x", position.x()}, {"y", position.y()}};
  j["itemFlags"] = p.itemFlags();
  j["zValue"] = p.zValue();
  j["type"] = p.m_type.toStdString();
  j["shortname"] = p.m_shortname.toStdString();
  j["displayname"] = p.mDisplayName.toStdString();
  j["proxy"] = p.m_proxy;
  j["iconTop"] = p.iconTop;
  j["iconLeft"] = p.iconLeft;
  nlohmann::json ports;
  for (const auto &pt : p.m_ports.keys()) {
    QString key = QString::fromUtf8(pt);
    ports[key.toStdString()] = *p.m_ports[pt];
  }
  j["ports"] = ports;
  j["annotation"] = p.m_annotation;
  const SceneItem &bgi = dynamic_cast<const SceneItem &>(p);
  j["sceneitem"] = bgi;
}

void from_json(const nlohmann::json &j, BGElement &p) {
  SceneItem &bgi = dynamic_cast<SceneItem &>(p);
  from_json(j["sceneitem"], bgi);
  p.setId(QString::fromStdString(j["mid"]));
  p.m_type = QString::fromStdString(j["type"]);
  p.setSize(j["size"]["width"], j["size"]["height"]);
  p.setItemFlags(j["itemFlags"]);
  p.setZValue(j["zValue"]);
  p.m_shortname = QString::fromStdString(j["shortname"]);
  p.iconTop = j["iconTop"];
  p.iconLeft = j["iconLeft"];
  // Create ports
  for (const auto &pt : j["ports"].items()) {
    QString key = QString::fromStdString(pt.key());
    BGPort *port = p.addPort(key.toUtf8());
    const nlohmann::json &pdes = pt.value();
    from_json(pdes, *port);
  }
  p.maxPorts = j["maxPorts"];
  p.m_proxy = j["proxy"];
  p.m_annotation = j["annotation"];
  p.m_annotationCache = QString::fromStdString(p.m_annotation.dump()).toUtf8();
  // Set the display name and set the icon
  p.setDisplayName(QString::fromStdString(j["displayname"]));
  p.updateCachedItems();
}

void to_json(nlohmann::json &j, const BGPort &p) {
  j["id"] = p.m_id.toStdString();
  j["align"] = p.m_align;
  j["canDelete"] = p.m_canDelete;
  j["direction"] = p.m_direction;
  j["anchorSize"] = {{"x", p.m_anchorSize.x()},
                     {"y", p.m_anchorSize.y()},
                     {"width", p.m_anchorSize.width()},
                     {"height", p.m_anchorSize.height()}};
  j["position"] = {{"x", p.m_position.x()}, {"y", p.m_position.y()}};
  j["colour"] = p.getColor().name().toStdString();
}

void from_json(const nlohmann::json &j, BGPort &p) {
  p.m_id = QString::fromStdString(j["id"]).toUtf8();
  p.m_canDelete = j["canDelete"];
  p.setDirection(j["direction"]);
  const auto &aSize = j["anchorSize"];
  QRectF as(aSize["x"], aSize["y"], aSize["width"], aSize["height"]);
  p.m_anchorSize = as;

  if (p.m_canDelete) {
    const auto &position = j["position"];
    QPointF pos(position["x"], position["y"]);
    p.setAlignment(j["align"]);
    p.setColor(QColor(QString::fromStdString(j["colour"])));
    p.setPos(pos);
  } else {
    p.setAlignment(Qt::AlignCenter);
    p.setColor(QColor(Qt::transparent));
  }
}

void to_json(nlohmann::json &j, const SceneItem &p) {
  // j["id"] = p.m_id.toStdString();
  j["label"] = p.m_labelItem->text().toStdString();
  nlohmann::json attrib;

  for (const auto &attr : p.m_attributes.keys()) {
    QString key = QString::fromUtf8(attr);
    const QVariant &val = p.m_attributes[attr];
    if (val.type() == QVariant::String) {
      attrib[key.toStdString()] = val.toString().toStdString();
    } else {
      attrib[key.toStdString()] = val.toDouble();
    }
  }
  j["attrib"] = attrib;
}

void from_json(const nlohmann::json &j, SceneItem &p) {
  p.m_labelItem->setText(QString::fromStdString(j["label"]));
  const nlohmann::json &attrib = j["attrib"];
  for (const auto &ats : attrib.items()) {
    const nlohmann::json &val = ats.value();
    if (val.is_string()) {
      p.m_attributes[QString::fromStdString(ats.key()).toUtf8()] =
          QVariant(QString::fromStdString(val.get<std::string>()));
    } else {
      p.m_attributes[QString::fromStdString(ats.key()).toUtf8()] =
          QVariant(val.get<double>());
    }
  }
}

std::tuple<RCPLIB::RCP<BG::BondGraphInterface>,
           std::map<std::string, RCPLIB::RCP<BG::BGElement>>>
loadBondGraph(nlohmann::json &res) {
  static nlohmann::json supportedMethods =
      BG::getSupportedPhysicalDomainsAndFactoryMethods();
  auto items = res["sceneItems"];
  std::ostringstream ss;
  std::map<std::string, RCPLIB::RCP<BG::BGElement>> elements;
  if (items.size() == 0) {
    BG::BondGraphInterface *inter = nullptr;
    return std::make_tuple(RCPLIB::rcp(inter), elements);
  }
  // Clear workspace
  BG::newWorkSpace();
  auto ioBondGraph = BG::createBondGraph();
  std::map<std::string, std::vector<std::string>> bondgraphImportName;
  std::map<std::string, RCPLIB::RCP<BG::BondGraphInterface>>
      elementBondGraphMap;
  std::map<std::string, std::map<std::string, RCPLIB::RCP<BG::BGElement>>>
      bondGraphElements;
  std::map<std::string, std::vector<std::string>> parameterCellMLs;
  // std::map<std::string, std::vector<std::string>> stateCellMLs;
  // Create the bondgraphs
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    if (val["typeId"] == "BGElement") {
      auto def = val["definition"];
      auto anot = def["annotation"];
      auto edef = anot["ElementDefinition"];
      auto sp = anot["statesAndParameters"];
      auto dName = def["displayname"];
      std::string domain = edef["domain"];
      std::string type = edef["type"];
      std::string clas = edef["class"];
      // Find cellml
      if (clas ==
          "passive") { // If passive set prescribed state and parameter values
        for (const auto &pm : sp) {
          auto nm = pm["name"];
          if (pm["value"].is_object()) {
            // if (edef.contains("states")) {
            //   if (edef["states"].contains(
            //           nm)) { // For states no mapping, pull the values
            //     if (stateCellMLs.find(pm["filename"]) != stateCellMLs.end())
            //     {
            //       stateCellMLs[pm["filename"]].push_back(nm);
            //     } else {
            //       stateCellMLs[pm["filename"]] = {nm};
            //     }
            //     continue;
            //   }
            // }
            if (parameterCellMLs.find(pm["filename"]) !=
                parameterCellMLs.end()) {
              parameterCellMLs[pm["filename"]].push_back(nm);
            } else {
              parameterCellMLs[pm["filename"]] = {nm};
            }
          }
        }
      }

      std::string mid = def["mid"];
      if (anot.contains("proxy")) {
        auto proxy = anot["proxy"];
        bool link = proxy["link"];
        std::string fname = proxy["uri"];
        std::string basefname = fname.substr(fname.find_last_of("/\\") + 1);
        std::string basename = basefname.substr(0, basefname.find_last_of("."));

        if (!link) {
          std::ifstream ifs(fname.c_str());
          auto bjson = nlohmann::json::parse(ifs);
          auto resB = loadBondGraph(bjson);
          if (!std::get<0>(resB).is_null()) {
            if (bondgraphImportName.find(basename) !=
                bondgraphImportName.end()) {
              bondgraphImportName[basename] = {mid};
            } else {
              bondgraphImportName[basename].push_back(mid);
            }
            std::string linkName = basename + std::string("_") + mid;
            elementBondGraphMap[linkName] = std::get<0>(resB);
            bondGraphElements[linkName] = std::get<1>(resB);
          }
        }
      }
    }
  }
  // Create elements and proxies
  std::map<std::string, RCPLIB::RCP<BG::BondGraphInterface>> bgs;
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    if (val["typeId"] == "BGElement") {
      auto def = val["definition"];
      auto anot = def["annotation"];
      auto edef = anot["ElementDefinition"];
      auto dName = def["displayname"];
      std::string domain = edef["domain"];
      std::string type = edef["type"];
      std::string clas = edef["class"];
      std::string mName = supportedMethods[domain][type];
      RCPLIB::RCP<BG::BGElement> bge;
      if (!anot.contains("proxy")) {
        bge = BG::createBondgraphElement(mName);
      } else {
        auto proxy = anot["proxy"];
        std::string fname = proxy["uri"];
        std::string basefname = fname.substr(fname.find_last_of("/\\") + 1);
        std::string basename = basefname.substr(0, basefname.find_last_of("."));
        std::string refName = proxy["mid"];
        std::string linkName = basename + std::string("_") + refName;
        auto bg = elementBondGraphMap[linkName];
        auto elems = bondGraphElements[linkName];
        bge = BG::createProxy(elems[refName]);
        bgs[linkName] = bg;
      }
      elements[dName] = bge;
      ioBondGraph->addComponent(bge);
    }
  }
  for (auto &b : bgs) {
    ioBondGraph->addBondGraph(b.second);
  }

  // Set state and parameter values
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    if (val["typeId"] == "BGElement") {
      auto def = val["definition"];
      auto anot = def["annotation"];
      auto edef = anot["ElementDefinition"];
      auto sp = anot["statesAndParameters"];
      auto dName = def["displayname"];
      std::string domain = edef["domain"];
      std::string type = edef["type"];
      std::string clas = edef["class"];
      std::string mName = supportedMethods[domain][type];
      RCPLIB::RCP<BG::BGElement> bge = elements[dName];

      if (clas ==
          "passive") { // If passive set prescribed state and parameter values
        for (const auto &pm : sp) {
          std::string nm = pm["name"];
          if (pm["value"].is_object()) {
            // pm["value"]["value"] Name should be mangled to map to file
            // std::vector<std::string> &iVec = stateCellMLs[pm["filename"]];
            // if (std::find(iVec.begin(), iVec.end(), nm) != iVec.end()) {
            //   auto vars = Utils::getCellMLOutVariables(
            //       QString::fromStdString(pm["filename"]));
            //   std::string comp = pm["component"];
            //   std::string tv = comp + std::string(":") + nm;
            //   QList<QString> &v = vars[QString::fromStdString(tv)];
            //   bge->setParameter(nm, v[3].toStdString(),
            //                     pm["value"]["dimension"]);
            // } else {
            //   nlohmann::json vmap;
            //   vmap["type"] = "file";
            //   vmap["compartment"] = pm["component"];
            //   std::string fileName = pm["filename"];
            //   std::string baseFilename =
            //       fileName.substr(fileName.find_last_of("/\\") + 1);
            //   vmap["filename"] = baseFilename;
            //   vmap["target"] = pm["variable"];
            //   // vmap["importname"] = ""; //Provide an import name for this
            //   // cellml file, use for resolving multiple/shared instances
            //   bge->setParameter(nm, vmap.dump(), pm["value"]["dimension"]);
            // }
            nlohmann::json vmap;
            vmap["type"] = "file";
            vmap["compartment"] = pm["component"];
            std::string fileName = pm["filename"];
            std::string baseFilename =
                fileName.substr(fileName.find_last_of("/\\") + 1);
            vmap["filename"] = baseFilename;
            vmap["target"] = pm["variable"];
            if (pm.contains("stateValue")) {
              vmap["stateValue"] = pm["stateValue"];
            }
            // vmap["importname"] = ""; //Provide an import name for this
            // cellml file, use for resolving multiple/shared instances
            bge->setParameter(nm, vmap.dump(), pm["value"]["dimension"]);
          } else {
            ss.str("");
            ss << pm["value"];
            bge->setParameter(nm, ss.str(), pm["dimension"]);
          }
        }
      }
    }
  }

  // Create connections
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    if (val["typeId"] == "BGDConnection") {
      auto def = val["definition"];
      auto fElem = elements[def["first"]];
      auto fPort = def["firstPort"];
      auto fports = fElem->getPorts();
      auto cFP = 0;
      for (size_t pc = 0; pc < fports.size(); pc++) {
        if (fports[pc]->getId() == fPort) {
          cFP = pc;
          break;
        }
      }

      auto lElem = elements[def["second"]];
      auto lPort = def["secondPort"];
      auto lports = lElem->getPorts();
      auto cLP = 0;
      for (size_t pc = 0; pc < lports.size(); pc++) {
        if (lports[pc]->getId() == lPort) {
          cLP = pc;
          break;
        }
      }
      ioBondGraph->connect(fElem, cFP, lElem,
                           cLP); // Inverting is false by default
    }
  }

  return std::make_tuple(ioBondGraph, elements);
}

QList<QString> getCellMLImports(std::string file) {
  QList<QString> imports;
  QFile f(QString::fromStdString(file));
  if (!f.open(QIODevice::ReadOnly)) {
    // Error while loading file
    throw std::runtime_error("Error while loading file " + file);
  }
  QDomDocument xmlBOM;
  // Set data into the QDomDocument before processing
  xmlBOM.setContent(&f);
  f.close();

  QDomElement Component = xmlBOM.documentElement().firstChild().toElement();

  // Loop while there is a child
  while (!Component.isNull()) {
    // Check if the child tag name is COMPONENT
    if (Component.tagName() == "import" || Component.tagName() == "IMPORT") {

      // Read link
      QString link = Component.attribute("xlink:href");
      imports.append(link);
    }
    // Next component
    Component = Component.nextSibling().toElement();
  }
  return imports;
}

void copyDependentCellMLFiles(std::map<std::string, std::string> &files,
                              QDir dir, QDir ldir) {
  // If the project was loaded from a different directory to the initial ones
  // hard coded file names will change
  bool handleEmbeddedFilename = dir.absolutePath() != ldir.absolutePath();
  // qDebug() << dir << "\t" << ldir;
  QMap<QString, QString> imports;
  for (const auto &f : files) {
    std::string SRC = f.second;
    QFileInfo srcInfo(QString::fromStdString(SRC));
    QString baseFilename = srcInfo.fileName();
    if (handleEmbeddedFilename) {
      // qDebug() << Q_FUNC_INFO << "\n"
      //          << QString::fromStdString(SRC) << "\t changed to "
      //          << ldir.absoluteFilePath(baseFilename);
      SRC = ldir.absoluteFilePath(baseFilename).toStdString();
      srcInfo = QFileInfo(QString::fromStdString(SRC));
    }

    // Check if the file exists; note that file names are hardcoded, and when
    // the directory is moved this will lead to file not found issues, so try
    // and change the name with respect the loaded directory

    QString fileDir = srcInfo.path();
    auto fimports = getCellMLImports(SRC);
    for (QString &fx : fimports) {
      // Check if filename is absolute
      QFileInfo qfInfo(fx);
      QString qf = qfInfo.fileName();
      QString qfp = qfInfo.filePath();
      if (qf == qfp) { // Then f is a file name, should be qualifiled with dir
        imports[qf] = dir.filePath(qf);
      } else {
        imports[qf] = qfp;
      }
      // qDebug()<< baseFilename<<" located at "<<fileDir << " has dependency on
      // "<<qf<<" located at "<< imports[qf];
    }

    imports[QString::fromStdString(f.first)] = srcInfo.filePath();
  }
  std::ostringstream failedFiles;
  for (auto &f : imports.toStdMap()) {
    try {
      std::string DEST = dir.filePath(f.first).toStdString();
      std::string SRC = f.second.toStdString();
      if (SRC != DEST) {
        std::ifstream src(SRC.c_str());
        std::ofstream dest(DEST.c_str());
        dest << src.rdbuf();
        dest.close();
        src.close();
      }
    } catch (...) {
      failedFiles << f.second.toStdString() << "\n";
    }
  }
  std::string failedFileString = failedFiles.str();
  if (failedFileString.size() > 0) {
    throw std::runtime_error(failedFileString);
  }
}

RCPLIB::RCP<BG::BondGraphInterface>
getBondGraphFor(nlohmann::json &items,
                std::map<std::string, std::string> &importFile) {
  nlohmann::json methods = BG::getSupportedPhysicalDomainsAndFactoryMethods();
  std::map<std::string, RCPLIB::RCP<BG::BGElement>> elements;
  std::ostringstream ss;
  auto ioBondGraph = BG::createBondGraph();
  // Find all the links and the shared ones
  std::map<std::string, std::string> importName;
  std::map<std::string, std::map<std::string, std::string>> sharedImports;
  std::map<std::string,
           std::map<std::string, RCPLIB::RCP<BG::BondGraphInterface>>>
      bondgraphInstances;
  std::map<std::string, RCPLIB::RCP<BG::BGElement>> proxies;
  std::vector<std::string> missingElements;
  for (const auto &itm : items.items()) {
    std::string k = itm.key();
    auto val = itm.value();

    if (val["typeId"] == "BGElement") {
      auto def = val["definition"];
      auto edef = def["annotation"]["ElementDefinition"];
      auto sp = def["annotation"]["statesAndParameters"];
      auto dName = def["displayname"];
      std::string domain = edef["domain"];
      std::string type = edef["type"];
      std::string clas = edef["class"];

      // Handle proxies
      if (!def["annotation"].contains("proxy")) {
        if (clas == "userdefined") {
          auto mapDef = def["annotation"]["mapdefinition"];
          // Cut to basename
          std::string fileName = mapDef["uri"];
          std::string baseFilename =
              fileName.substr(fileName.find_last_of("/\\") + 1);
          importFile[baseFilename] = fileName;
          if (sharedImports.find(baseFilename) != sharedImports.end()) {
            auto &links = sharedImports[baseFilename];
            // element name, import name
            if (mapDef["link"]) {
              std::string elementName = mapDef["element"];
              if (links.find(elementName) != links.end()) {
                importName[k] = links[elementName];
              } else {
                // Should match with line 705, so that there is no clash, else
                // we have to backfil
                importName[k] = elementName + "_CML";
              }
            } else {
              importName[k] = k + "_CML";
            }
          } else {
            std::map<std::string, std::string> mp;
            if (mapDef["link"]) {
              std::string elementName = mapDef["element"];
              // Should match with line 795, so that there is no clash, else
              // we have to backfil
              importName[k] = elementName + "_CML";
            } else {
              importName[k] = k + "_CML";
            }
            mp[k] = importName[k];
            sharedImports[baseFilename] = mp;
          }
        } else if (clas == "junction") {
          // Same for TF, GY junctions as well
          if (sp.is_object() || sp.is_array()) {
            for (const auto &pm : sp) {
              auto nm = pm["name"];
              if (pm["value"].is_object()) {
                auto pv = pm["value"];
                std::string fileName = pv["file"];
                std::string baseFilename =
                    fileName.substr(fileName.find_last_of("/\\") + 1);
                importFile[baseFilename] = fileName;
                if (sharedImports.find(baseFilename) != sharedImports.end()) {
                  auto &links = sharedImports[baseFilename];
                  // element name, import name
                  if (pv["link"]) {
                    std::string elementName = pv["element"];
                    if (links.find(elementName) != links.end()) {
                      importName[k] = links[elementName];
                    } else {
                      // Should match with line 705, so that there is no
                      // clash, else we have to backfil
                      importName[k] = elementName + "_CML";
                    }
                  } else {
                    importName[k] = k + "_CML";
                  }
                } else {
                  std::map<std::string, std::string> mp;
                  if (pv["link"]) {
                    std::string elementName = pv["element"];
                    // Should match with line 849, so that there is no clash,
                    // else we have to backfil
                    importName[k] = elementName + "_CML";
                  } else {
                    importName[k] = k + "_CML";
                  }
                  mp[k] = importName[k];
                  sharedImports[baseFilename] = mp;
                }
              }
            }
          }
        } else if (clas == "passive") {
          for (const auto &pm : sp) {
            auto nm = pm["name"];
            if (pm["value"].is_object()) {
              auto pv = pm["value"];
              if (!pv.contains("file"))
                continue;
              std::string fileName = pv["file"];
              std::string baseFilename =
                  fileName.substr(fileName.find_last_of("/\\") + 1);
              importFile[baseFilename] = fileName;
              // Cut to basename
              // Check if any varibles need to be mapped
              if (sharedImports.find(baseFilename) != sharedImports.end()) {
                auto &links = sharedImports[baseFilename];
                // element name, import name
                if (pv["link"]) {
                  std::string elementName = pv["element"];
                  if (links.find(elementName) != links.end()) {
                    importName[k] = links[elementName];
                  } else {
                    // Should match with line 705, so that there is no clash,
                    // else we have to backfil
                    importName[k] = elementName + "_CML";
                  }
                } else {
                  importName[k] = k + "_CML";
                }
              } else {
                std::map<std::string, std::string> mp;
                if (pv["link"]) {
                  std::string elementName = pv["element"];
                  // Should match with line 705, so that there is no clash,
                  // else we have to backfil
                  importName[k] = elementName + "_CML";
                } else {
                  importName[k] = k + "_CML";
                }
                mp[k] = importName[k];
                sharedImports[baseFilename] = mp;
              }
            }
          }
        }
      } else {
        auto proxy = def["annotation"]["proxy"];
        std::string fname = proxy["uri"];
        std::string basefname = fname.substr(fname.find_last_of("/\\") + 1);
        std::string basename = basefname.substr(0, basefname.find_last_of("."));
        // When elements are created, their name is set to displayname and this
        // is returned when getComponents is called
        QString displayName = QString::fromStdString(proxy["mid"])
                                  .replace("^", "_p_")
                                  .replace("{", "")
                                  .replace("}", "")
                                  .replace("0", "O")
                                  .replace("1", "I")
                                  .replace(":", "_c_");
        std::string refName = displayName.toStdString();
        std::string linkName = basename + std::string("_") + refName;
        // bondgraphInstances filename, map<elementName, instance> >
        RCPLIB::RCP<BG::BondGraphInterface> bgraph;
        if (bondgraphInstances.find(basefname) != bondgraphInstances.end()) {
          auto &links = bondgraphInstances[basefname];
          // element name, import name
          if (proxy["link"]) {
            std::string elementName = proxy["element"];
            if (links.find(elementName) != links.end()) {
              bgraph = links[elementName];
            } else {
              // Create one for that element and link
              std::ifstream ijs(fname);
              nlohmann::json j = nlohmann::json::parse(ijs);
              bgraph = getBondGraphFor(j["sceneItems"], importFile);
              links[elementName] = bgraph;
              ioBondGraph->addBondGraph(bgraph);
            }
          } else {
            std::ifstream ijs(fname);
            nlohmann::json j = nlohmann::json::parse(ijs);
            bgraph = getBondGraphFor(j["sceneItems"], importFile);
            links[dName] = bgraph;
            ioBondGraph->addBondGraph(bgraph);
          }
        } else {
          std::map<std::string, RCPLIB::RCP<BG::BondGraphInterface>> links;
          // element name, import name
          if (proxy["link"]) {
            std::string elementName = proxy["element"];
            // Create one for that element and link
            std::ifstream ijs(fname);
            nlohmann::json j = nlohmann::json::parse(ijs);
            bgraph = getBondGraphFor(j["sceneItems"], importFile);
            links[elementName] = bgraph;
            ioBondGraph->addBondGraph(bgraph);
          } else {
            std::ifstream ijs(fname);
            nlohmann::json j = nlohmann::json::parse(ijs);
            bgraph = getBondGraphFor(j["sceneItems"], importFile);
            links[dName] = bgraph;
            ioBondGraph->addBondGraph(bgraph);
          }
          bondgraphInstances[basefname] = links;
        }
        // Get the target element and create a proxy
        try {
          auto px = bgraph->getElementByName(refName);
          proxies[dName] = px;
        } catch (std::exception &ex) {
          Q_UNUSED(ex);
          // Do error check
          missingElements.push_back(refName);
        }
      }
    }
  }
  if (missingElements.size() > 0) {
    QString msg = "Failed to find proxy elements(s):\n";
    for (auto m : missingElements) {
      msg += QString::fromStdString(m) + "\n";
    }
    throw msg.toStdString();
  }
  // Create the elements
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    std::string assignedImportName = "";
    if (importName.find(k) != importName.end()) {
      assignedImportName = importName[k];
    }
    if (val["typeId"] == "BGElement") {
      auto def = val["definition"];
      auto edef = def["annotation"]["ElementDefinition"];
      auto sp = def["annotation"]["statesAndParameters"];
      QString displayName = QString::fromStdString(def["displayname"])
                                .replace("^", "_p_")
                                .replace("{", "")
                                .replace("}", "")
                                .replace("0", "O")
                                .replace("1", "I")
                                .replace(":", "_c_");

      auto cellmlName = displayName.toStdString();
      auto dName = def["mid"]; // Connections store mid's

      std::string domain = edef["domain"];
      std::string type = edef["type"];
      std::string clas = edef["class"];
      std::string mName = methods[domain][type];

      RCPLIB::RCP<BG::BGElement> bge;
      if (proxies.find(dName) != proxies.end()) {
        bge = proxies[dName];
      }
      if (clas == "userdefined") {
        auto mapDef = def["annotation"]["mapdefinition"];

        nlohmann::json vmap;
        vmap["type"] = "file";
        vmap["compartment"] = mapDef["component"];
        // Cut to basename
        std::string fileName = mapDef["uri"];
        std::string baseFilename =
            fileName.substr(fileName.find_last_of("/\\") + 1);
        importFile[baseFilename] = fileName;
        vmap["filename"] = baseFilename;
        vmap["target"] = mapDef["variable"];
        vmap["link"] = mapDef["link"];
        // If link is true provide "importName" name so that the same cellml
        // instance is linked
        //||assignedImportName!="" part is to ensure the main varible uses
        // the
        // same importName
        if (vmap["link"] || assignedImportName != "") {
          vmap["importName"] = assignedImportName;
        }
        if (mapDef.contains("timeVariable")) {
          vmap["mapvariables"] = mapDef["timeVariable"];
        }
        // Check if any varibles need to be mapped
        if (mapDef["sourceType"]) { // Potential type
          if (bge.is_null())
            bge = BG::createPotentialSource();
          bge->setParameter("u", vmap.dump(), mapDef["dimension"]);
          bge->setName(cellmlName);
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << "createPotentialSource "
                   << QString::fromStdString(bge->getName()) << "\n"
                   << QString::fromStdString(vmap.dump()) << "\n"
                   << QString::fromStdString(mapDef["dimension"]);
#endif
        } else {
          if (bge.is_null())
            bge = BG::createFlowSource();
          bge->setName(cellmlName);
          bge->setParameter("i", vmap.dump(), mapDef["dimension"]);
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << "createPotentialSource "
                   << QString::fromStdString(bge->getName()) << "\n"
                   << QString::fromStdString(vmap.dump()) << "\n"
                   << QString::fromStdString(mapDef["dimension"]);
#endif
        }
      } else if (clas == "junction") { // Handle TF and GY
        if (bge.is_null())
          bge = BG::createBondgraphElement(mName);
        bge->setName(cellmlName);
#ifdef DEBUG_CELLML_EXPORT
        qDebug() << "create junction " << QString::fromStdString(bge->getName())
                 << "\n"
                 << QString::fromStdString(mName);
#endif
        if (sp.is_object() || sp.is_array()) {
          for (const auto &pv : sp) {
            auto nm = pv["name"];
            if (pv["value"].is_object()) {
              auto pm = pv["value"];
              if (pm.contains("file")) { // When a file reference is made
                nlohmann::json vmap;
                vmap["type"] = "file";
                vmap["compartment"] = pm["component"];
                std::string fileName = pm["filename"];
                std::string baseFilename =
                    fileName.substr(fileName.find_last_of("/\\") + 1);
                vmap["filename"] = baseFilename;
                vmap["target"] = pm["variable"];
                vmap["link"] = pm["link"];
                // If link is true provide "importName" name so that the
                // same cellml instance is linked
                if (vmap["link"] || assignedImportName != "") {
                  vmap["importName"] = assignedImportName;
                }
                if (pm.contains("timeVariable")) {
                  vmap["mapvariables"] = pm["timeVariable"];
                }
                // file, use for resolving multiple/shared instances
                bge->setParameter(nm, vmap.dump(), "");
#ifdef DEBUG_CELLML_EXPORT
                qDebug() << "\t parameter " << QString::fromStdString(nm)
                         << "\n"
                         << QString::fromStdString(vmap.dump());
#endif
              } else { // User has changed just the value
                ss.str("");
                ss << pv["value"]["value"];
                bge->setParameter(nm, ss.str(), "");
#ifdef DEBUG_CELLML_EXPORT
                qDebug() << "\t parameter " << QString::fromStdString(nm)
                         << "\n"
                         << QString::fromStdString(ss.str());
#endif
              }
            } else {
              ss.str("");
              ss << pv["value"];
              bge->setParameter(nm, ss.str(), "");
#ifdef DEBUG_CELLML_EXPORT
              qDebug() << "\t parameter " << QString::fromStdString(nm) << "\n"
                       << QString::fromStdString(ss.str());
#endif
            }
          }
        }
      } else if (clas == "passive") { // If passive set prescribed state and
        // parameter values
        if (bge.is_null())
          bge = BG::createBondgraphElement(mName);
        bge->setName(cellmlName);
#ifdef DEBUG_CELLML_EXPORT
        qDebug() << "create passive " << QString::fromStdString(bge->getName())
                 << "\n"
                 << QString::fromStdString(mName);
#endif

        for (const auto &pv : sp) {
          auto nm = pv["name"];
          if (pv["value"].is_object()) {
            auto pm = pv["value"];
            if (pm.contains("file")) { // When a file reference is made
              nlohmann::json vmap;
              vmap["type"] = "file";
              vmap["compartment"] = pm["component"];
              std::string fileName = pm["file"];
              std::string baseFilename =
                  fileName.substr(fileName.find_last_of("/\\") + 1);
              vmap["filename"] = baseFilename;
              vmap["target"] = pm["variable"];
              // Check if any varibles need to be mapped
              if (pm.contains("mapvariables")) {
                vmap["mapvariables"] = pm["mapvariables"]["timeVariable"];
              }
              vmap["link"] = pm["link"];
              // If link is true provide "importName" name so that the same
              // cellml instance is linked
              if (vmap["link"] || assignedImportName != "") {
                vmap["importName"] = assignedImportName;
              }
              if (pm.contains("timeVariable")) {
                vmap["mapvariables"] = pm["timeVariable"];
              }
              // file, use for resolving multiple/shared instances
              bge->setParameter(nm, vmap.dump(), pv["dimension"]);
#ifdef DEBUG_CELLML_EXPORT
              qDebug() << "\t parameter " << QString::fromStdString(nm) << "\n"
                       << QString::fromStdString(vmap.dump()) << "\n"
                       << QString::fromStdString(pv["dimension"]);
#endif
            } else { // User has changed just the value
              ss.str("");
              ss << pv["value"]["value"];
              bge->setParameter(nm, ss.str(), pv["dimension"]);
#ifdef DEBUG_CELLML_EXPORT
              qDebug() << "\t parameter " << QString::fromStdString(nm) << "\n"
                       << QString::fromStdString(ss.str()) << "\n"
                       << QString::fromStdString(pv["dimension"]);
#endif
            }
          } else {
            ss.str("");
            ss << pv["value"];
            bge->setParameter(nm, ss.str(), pv["dimension"]);
#ifdef DEBUG_CELLML_EXPORT
            qDebug() << "\t parameter " << QString::fromStdString(nm) << "\n"
                     << QString::fromStdString(ss.str()) << "\n"
                     << QString::fromStdString(pv["dimension"]);
#endif
          }
        }
      }
      elements[dName] = bge;
      ioBondGraph->addComponent(bge);
    }
  }
  // Create the connections
  for (const auto &itm : items.items()) {
    auto k = itm.key();
    auto val = itm.value();
    if (val["typeId"] == "BGDConnection") {
      auto def = val["definition"];
      auto fElem = elements[def["first"]];
      auto fPort = def["firstPort"];
      auto fports = fElem->getPorts();
      auto cFP = 0;
#ifdef DEBUG_CELLML_EXPORT1
      qDebug() << Q_FUNC_INFO << "\t" << QString::fromStdString(def["first"])
               << "\t" << fports.size();
#endif
      for (size_t pc = 0; pc < fports.size(); pc++) {
        if (fports[pc]->getId() == fPort) {
          cFP = pc;
          break;
        }
      }
#ifdef DEBUG_CELLML_EXPORT1
      for (size_t pc = 0; pc < fports.size(); pc++) {
        qDebug() << " Connecting " << QString::fromStdString(fElem->getName())
                 << " port " << pc << "\t PID "
                 << QString::fromStdString(fports[pc]->getId()) << "\t match "
                 << QString::fromStdString(fPort);
      }
#endif
      auto lElem = elements[def["second"]];
      auto lPort = def["secondPort"];
      auto lports = lElem->getPorts();
      auto cLP = 0;
      for (size_t pc = 0; pc < lports.size(); pc++) {
        if (lports[pc]->getId() == lPort) {
          cLP = pc;
          break;
        }
      }
#ifdef DEBUG_CELLML_EXPORT1
      for (size_t pc = 0; pc < lports.size(); pc++) {
        qDebug() << " Connecting " << QString::fromStdString(lElem->getName())
                 << " port " << pc << "\t PID "
                 << QString::fromStdString(lports[pc]->getId()) << "\t match "
                 << QString::fromStdString(lPort);
      }
#endif
      auto fType = fElem->getType();
      auto lType = lElem->getType();
      bool fElemISReaction = fType == BG::PassiveType::bReaction;
      bool lElemISReaction = lType == BG::PassiveType::bReaction;
      bool fElemISJunction =
          fType == BG::PassiveType::eOne || fType == BG::PassiveType::eZero;
      bool lElemISJunction =
          lType == BG::PassiveType::eOne || lType == BG::PassiveType::eZero;
#ifdef DEBUG_CELLML_EXPORT1
      qDebug() << " Type " << QString::fromStdString(fElem->getName()) << "\t"
               << fElemISJunction;
      qDebug() << " Type " << QString::fromStdString(lElem->getName()) << "\t"
               << lElemISJunction;
#endif
      if (!fElemISReaction && !lElemISReaction) {
        if (!fElemISJunction && !lElemISJunction) {
          ioBondGraph->connect(fElem, cFP, lElem, cLP);
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << QString::fromStdString(k) << " Connecting "
                   << QString::fromStdString(fElem->getName()) << " port "
                   << cFP << "\n"
                   << QString::fromStdString(lElem->getName()) << " port "
                   << cLP;
#endif
        } else if (fElemISJunction && !lElemISJunction) {
          ioBondGraph->connect(
              fElem, lElem, 0,
              cLP); // Match the signature to avoid the ambiguity
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << QString::fromStdString(k) << " Connecting "
                   << QString::fromStdString(fElem->getName()) << " Any port "
                   << "\n"
                   << QString::fromStdString(lElem->getName()) << " port "
                   << cLP;
#endif
        } else if (!fElemISJunction && lElemISJunction) {
          ioBondGraph->connect(fElem, cFP, lElem);
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << QString::fromStdString(k) << " Connecting "
                   << QString::fromStdString(fElem->getName()) << " port "
                   << cFP << "\n"
                   << QString::fromStdString(lElem->getName()) << " Any port ";
#endif
        } else {
          ioBondGraph->connect(fElem, lElem);
#ifdef DEBUG_CELLML_EXPORT
          qDebug() << QString::fromStdString(k) << " Connecting "
                   << QString::fromStdString(fElem->getName()) << " Any port "
                   << "\n"
                   << QString::fromStdString(lElem->getName()) << " Any port ";
#endif
        }
      } else if (fElemISReaction) {
        ioBondGraph->connectInverting(fElem, 1, lElem);
#ifdef DEBUG_CELLML_EXPORT
        qDebug() << QString::fromStdString(k) << " ConnectInverting"
                 << QString::fromStdString(fElem->getName()) << " 1 "
                 << "\n"
                 << QString::fromStdString(lElem->getName()) << " Any port ";
#endif
      } else {
        ioBondGraph->connectInverting(fElem, lElem, 0);
#ifdef DEBUG_CELLML_EXPORT
        qDebug() << QString::fromStdString(k) << " ConnectInverting"
                 << QString::fromStdString(fElem->getName()) << " Any port "
                 << "\n"
                 << QString::fromStdString(lElem->getName()) << " 0 ";
#endif
      }
    }
  }
  return ioBondGraph;
}

QString to_cellML(QString name, QString dir, const BGEditorScene &p) {
  nlohmann::json res = p;
  QString serializedFileName ="";
  auto items = res["sceneItems"];
  if (items.size() == 0)
    return serializedFileName;

  nlohmann::json pdetails;
  QSettings bgSettings;
  bgSettings.beginGroup("BondGraphUI/Bondgraph/ProjectDetails");

  pdetails["projectname"] =
      bgSettings.value("Project_Name", "").toString().toStdString();
  pdetails["authors"] =
      bgSettings.value("Project_Authors", "").toString().toStdString();
  pdetails["description"] =
      bgSettings.value("Project_Description", "").toString().toStdString();
  QString loadedDirectory =
      bgSettings.value("Project_Directory", "").toString();

  bgSettings.endGroup();

  BGProjectDetails *details;
  if (name == "" || dir == "") {
    details = new BGProjectDetails(true, nullptr);
    details->setWindowTitle(QObject::tr("Provide project details"));
  } else {
    details = new BGProjectDetails(true, nullptr);
    details->setWindowTitle(QObject::tr("Check project details"));
  }
  if (details->exec() == QDialog::Accepted) {
    details->setCursor(Qt::WaitCursor);
    std::map<std::string, std::string> importFile;
    try {
      auto ioBondGraph = getBondGraphFor(items, importFile);

      auto eqs = ioBondGraph->computeStateEquation();

      if (eqs.bondGraphValidity) {

        nlohmann::json jres = details->getJson();
        std::string pdir = jres["directory"];
        QDir projectDirectory(QString::fromStdString(pdir));
        try {
          auto files = getCellML(jres["projectname"], ioBondGraph, eqs);
          for (const auto &i : files) {
            std::ofstream os;
            std::string filename =
                projectDirectory.filePath(QString::fromStdString(i.first))
                    .toStdString();
            if (filename == "StateEquationSolutions")
              continue;
            os.open(filename.c_str());
            os << i.second;
            os.close();
            // qDebug() <<
            // projectDirectory.filePath(QString::fromStdString(i.first));
          }
          serializedFileName =  projectDirectory.filePath(name+".cellml");
        } catch (std::exception &e) {
          details->setCursor(Qt::ArrowCursor);
          qDebug() << QString::fromStdString(e.what());
          QMessageBox::critical(
              nullptr, QObject::tr("Failed"),
              QObject::tr("Failed to generate CellML model for current "
                          "specification!!\n Bondgraph library returned %1")
                  .arg(QString::fromStdString(e.what())));
          details->setCursor(Qt::ArrowCursor);
          delete details;
          return serializedFileName;
        }
        try {
          copyDependentCellMLFiles(importFile, projectDirectory,
                                   QDir(loadedDirectory));
          // qDebug() <<Q_FUNC_INFO<<"\n"<<
          // QString::fromStdString(files[std::string(jres["projectname"])+".cellml"]);
          details->setCursor(Qt::ArrowCursor);
          QMessageBox::information(
              nullptr, QObject::tr("Success"),
              QObject::tr("A CellML model was successfully generated!"));
          details->setCursor(Qt::ArrowCursor);
          delete details;
          return serializedFileName;
        } catch (std::exception &e) {
          details->setCursor(Qt::ArrowCursor);
          qDebug() << QString::fromStdString(e.what());
          QMessageBox::critical(
              nullptr, QObject::tr("Missing dependency"),
              QObject::tr("%1\n Copy this file to %2, for the model to work!")
                  .arg(QString::fromStdString(e.what()),
                       projectDirectory.absolutePath()));
        }
      } else {
        details->setCursor(Qt::ArrowCursor);
        QMessageBox::critical(
            nullptr, QObject::tr("Invalid Bondgraph specification"),
            QObject::tr(
                "The Bondgraph logic identifies the specified bondgraph "
                "graphic is invalid!"));
      }
    } catch (std::exception &e) {
      details->setCursor(Qt::ArrowCursor);
      qDebug() << QString::fromStdString(e.what());
      QMessageBox::critical(
          nullptr, QObject::tr("Failed"),
          QObject::tr("Failed to generate CellML model for current "
                      "specification!!\n Bondgraph library returned %1")
              .arg(QString::fromStdString(e.what())));
      details->setCursor(Qt::ArrowCursor);
      delete details;
      return serializedFileName;
    }

    details->setCursor(Qt::ArrowCursor);
    delete details;
  }
  return serializedFileName;
}

} // namespace BondGraphUI
} // namespace OpenCOR