//==============================================================================
// CellML file element
//==============================================================================

#ifndef CELLMLFILEELEMENT_H
#define CELLMLFILEELEMENT_H

//==============================================================================

#include "cellmlfilerdftriple.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileElement
{
public:
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Model *pModel);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLImport *pCellmlImport);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ImportUnits *pImportUnits);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ImportComponent *pImportComponent);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Units *pUnits);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Unit *pUnit);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLComponent *pComponent);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLVariable *pVariable);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Group *pGroup);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::RelationshipRef *pRelationshipRef);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ComponentRef *pComponentRef);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Connection *pConnection);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::MapComponents *pMapComponents);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::MapVariables *pMapVariables);

    CellmlFileRdfTriples rdfTriples() const;

    CellMLSupport::CellmlFileRdfTriple * addMetadata(const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                     const QString &pResource,
                                                     const QString &pId);
    CellMLSupport::CellmlFileRdfTriple * addMetadata(const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                     const QString &pResource,
                                                     const QString &pId);

private:
    CellmlFile *mCellmlFile;

    iface::cellml_api::CellMLElement *mCellmlElement;

    QString mCmetaId;

    void generateUniqueCmetaId();
    QString rdfTripleSubject() const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
