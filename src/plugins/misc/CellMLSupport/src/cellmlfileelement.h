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
                               iface::cellml_api::Model *pCellmlApiModel);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLImport *pCellmlApiCellmlImport);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ImportUnits *pCellmlApiImportUnits);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ImportComponent *pCellmlApiImportComponent);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Units *pCellmlApiUnits);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Unit *pCellmlApiUnit);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLComponent *pCellmlApiComponent);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::CellMLVariable *pCellmlApiVariable);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Group *pCellmlApiGroup);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::RelationshipRef *pCellmlApiRelationshipRef);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::ComponentRef *pCellmlApiComponentRef);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::Connection *pCellmlApiConnection);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::MapComponents *pCellmlApiMapComponents);
    explicit CellmlFileElement(CellmlFile *pCellmlFile,
                               iface::cellml_api::MapVariables *pCellmlApiMapVariables);
    ~CellmlFileElement();

    CellmlFileRdfTriples rdfTriples() const;

    bool hasMetadata(const QString &pQualifier, const QString &pResource, const QString &pId);
    bool hasMetadata(const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                     const QString &pResource, const QString &pId);
    bool hasMetadata(const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                     const QString &pResource, const QString &pId);

    CellMLSupport::CellmlFileRdfTriple * addMetadata(const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                     const QString &pResource,
                                                     const QString &pId);
    CellMLSupport::CellmlFileRdfTriple * addMetadata(const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                     const QString &pResource,
                                                     const QString &pId);

    void removeAllMetadata();

private:
    CellmlFile *mCellmlFile;

    iface::cellml_api::CellMLElement *mCellmlElement;

    QString mCmetaId;

    QString rdfTripleSubject();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
