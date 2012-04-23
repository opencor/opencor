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
