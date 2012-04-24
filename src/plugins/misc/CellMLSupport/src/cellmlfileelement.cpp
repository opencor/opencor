//==============================================================================
// CellML file element
//==============================================================================

#include "cellmlfileelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::Model *pModel) :
    mCmetaId(QString::fromStdWString(pModel->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::CellMLImport *pCellmlImport) :
    mCmetaId(QString::fromStdWString(pCellmlImport->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::ImportUnits *pImportUnits) :
    mCmetaId(QString::fromStdWString(pImportUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::ImportComponent *pImportComponent) :
    mCmetaId(QString::fromStdWString(pImportComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::Units *pUnits) :
    mCmetaId(QString::fromStdWString(pUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::Unit *pUnit) :
    mCmetaId(QString::fromStdWString(pUnit->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::CellMLComponent *pComponent) :
    mCmetaId(QString::fromStdWString(pComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::CellMLVariable *pVariable) :
    mCmetaId(QString::fromStdWString(pVariable->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::Group *pGroup) :
    mCmetaId(QString::fromStdWString(pGroup->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::RelationshipRef *pRelationshipRef) :
    mCmetaId(QString::fromStdWString(pRelationshipRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::ComponentRef *pComponentRef) :
    mCmetaId(QString::fromStdWString(pComponentRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::Connection *pConnection) :
    mCmetaId(QString::fromStdWString(pConnection->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::MapComponents *pMapComponents) :
    mCmetaId(QString::fromStdWString(pMapComponents->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(iface::cellml_api::MapVariables *pMapVariables) :
    mCmetaId(QString::fromStdWString(pMapVariables->cmetaId()))
{
}

//==============================================================================

QString CellmlFileElement::cmetaId() const
{
    // Return the element's cmeta:id

    return mCmetaId;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
