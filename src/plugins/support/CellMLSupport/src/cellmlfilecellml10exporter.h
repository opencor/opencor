/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfilecellmlexporter.h"

//==============================================================================

#include <QPair>
#include <QSet>
#include <QString>

//==============================================================================

#include "cellmlapibegin.h"
    #include "IfaceAnnoTools.hxx"
    #include "IfaceCeVAS.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileCellml10Exporter : public CellmlFileCellmlExporter
{
public:
    explicit CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                        const QString &pFileName);

private:
    ObjRef<iface::cellml_services::AnnotationSet> mAnnotationSet;

    QSet<QPair<QString, QString>> mCopiedUnits;

    QSet<QString> mComponentNames;

    ObjRef<iface::cellml_api::Group> mRootGroup;

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
                   iface::cellml_api::ComponentRef *pToComponentReference = 0);
    void copyGroups(iface::cellml_api::Model *pModel);

    void copyConnection(iface::cellml_api::Connection *pConnection);
    void copyConnections(iface::cellml_api::Model *pModel);

    void propagateInitialValues();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
