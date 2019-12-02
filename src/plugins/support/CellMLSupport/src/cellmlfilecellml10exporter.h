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

//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileexporter.h"

//==============================================================================

#include <QPair>
#include <QSet>
#include <QString>

//==============================================================================

#include "cellmlapibegin.h"
    #include "cellml-api-cxx-support.hpp"

    #include "IfaceAnnoTools.hxx"
    #include "IfaceCeVAS.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileCellml10Exporter : public CellmlFileExporter
{
    Q_OBJECT

public:
    explicit CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                        const QString &pFileName);

private:
    iface::cellml_api::Model *mModel;
    ObjRef<iface::cellml_api::Model> mExportedModel;

    ObjRef<iface::cellml_services::AnnotationSet> mAnnotationSet;

    QSet<QPair<QString, QString>> mCopiedUnits;

    QSet<QString> mComponentNames;

    ObjRef<iface::cellml_api::Group> mRootGroup = nullptr;

    void copyUnitsSet(iface::cellml_api::UnitsSet *pUnitsSet,
                      iface::cellml_api::CellMLElement *pElement);

    iface::cellml_api::CellMLComponent * findRealComponent(iface::cellml_api::CellMLComponent *pComponent);

    void annotateImportedComponents(iface::cellml_api::Model *pModel);

    void ensureComponentNameUniqueness(std::wstring &pComponentName);

    iface::dom::Element * copyDomElement(iface::dom::Element *pDomElement);
    void copyExtensionElements(iface::cellml_api::CellMLElement *pFromElement,
                               iface::cellml_api::CellMLElement *pToElement);

    void copyComponent(iface::cellml_api::CellMLComponent *pComponent);
    void copyComponents(iface::cellml_services::CeVAS *pCevas);

    void copyGroup(iface::cellml_api::Model *pModel,
                   iface::cellml_api::ComponentRefSet *pComponentReferences,
                   iface::cellml_api::ComponentRef *pToComponentReference = nullptr);
    void copyGroups(iface::cellml_api::Model *pModel);

    void copyConnection(iface::cellml_api::Connection *pConnection);
    void copyConnections(iface::cellml_api::Model *pModel);

    void propagateInitialValues();

    bool saveModel(iface::cellml_api::Model *pModel, const QString &pFileName);
};

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
