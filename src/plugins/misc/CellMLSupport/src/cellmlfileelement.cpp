//==============================================================================
// CellML file element
//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfilemanager.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Model *pModel) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pModel),
    mCmetaId(QString::fromStdWString(pModel->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLImport *pCellmlImport) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pCellmlImport),
    mCmetaId(QString::fromStdWString(pCellmlImport->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportUnits *pImportUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportUnits),
    mCmetaId(QString::fromStdWString(pImportUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportComponent *pImportComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportComponent),
    mCmetaId(QString::fromStdWString(pImportComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Units *pUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnits),
    mCmetaId(QString::fromStdWString(pUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Unit *pUnit) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnit),
    mCmetaId(QString::fromStdWString(pUnit->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLComponent *pComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponent),
    mCmetaId(QString::fromStdWString(pComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLVariable *pVariable) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pVariable),
    mCmetaId(QString::fromStdWString(pVariable->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Group *pGroup) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pGroup),
    mCmetaId(QString::fromStdWString(pGroup->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::RelationshipRef *pRelationshipRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pRelationshipRef),
    mCmetaId(QString::fromStdWString(pRelationshipRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ComponentRef *pComponentRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponentRef),
    mCmetaId(QString::fromStdWString(pComponentRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Connection *pConnection) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pConnection),
    mCmetaId(QString::fromStdWString(pConnection->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapComponents *pMapComponents) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapComponents),
    mCmetaId(QString::fromStdWString(pMapComponents->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapVariables *pMapVariables) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapVariables),
    mCmetaId(QString::fromStdWString(pMapVariables->cmetaId()))
{
}

//==============================================================================

CellmlFileRdfTriples CellmlFileElement::rdfTriples() const
{
    // Return all the RDF triples associated with the element

    // Check that the element has a 'proper' cmeta:id

    if (mCmetaId.count())
        // The element has a 'proper' cmeta:id, so we can retrieve and return
        // the RDF triples associated with it

        return mCellmlFile->rdfTriples(mCmetaId);
    else
        // The element doesn't have a 'proper' cmeta:id, so...

        return CellmlFileRdfTriples();
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
