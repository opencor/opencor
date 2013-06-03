//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#ifndef CELLMLFILECELLML10EXPORTER_H
#define CELLMLFILECELLML10EXPORTER_H

//==============================================================================

#include "cellmlfileexporter.h"

//==============================================================================

#include <QPair>
#include <QSet>
#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceAnnoTools.hxx"
#include "IfaceCeVAS.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileCellml10Exporter : public CellmlFileExporter
{
public:
    explicit CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                        const QString &pFileName);

private:
    ObjRef<iface::cellml_services::AnnotationSet> mAnnotationSet;

    QSet<QPair<QString, QString> > mCopiedUnits;

    void copyUnitsSet(iface::cellml_api::UnitsSet *pUnitsSet,
                      iface::cellml_api::CellMLElement *pElement);

    iface::cellml_api::CellMLComponent * findRealComponent(iface::cellml_api::CellMLComponent *pComponent);

    void annotateImportedComponents(iface::cellml_api::Model *pModel);

    void copyComponents(iface::cellml_services::CeVAS *pCevas);

    void copyConnections(iface::cellml_api::Model *pModel);
    void copyGroups(iface::cellml_api::Model *pModel);

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
