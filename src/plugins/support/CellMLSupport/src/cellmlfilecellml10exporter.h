/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
