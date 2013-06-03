//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#ifndef CELLMLFILECELLML10EXPORTER_H
#define CELLMLFILECELLML10EXPORTER_H

//==============================================================================

#include "cellmlfilecellmlexporter.h"

//==============================================================================

#include <QPair>
#include <QSet>
#include <QString>

//==============================================================================

#include "IfaceAnnoTools.hxx"
#include "IfaceCeVAS.hxx"

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

    QSet<QPair<QString, QString> > mCopiedUnits;

    QSet<QString> mComponentNames;

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

    void copyConnections();
    void copyGroups();

    void propagateInitialValues();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
