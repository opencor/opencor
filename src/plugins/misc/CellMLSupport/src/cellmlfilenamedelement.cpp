//==============================================================================
// CellML file named element
//==============================================================================

#include "cellmlfilenamedelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::Model *pModel) :
    CellmlFileElement(pModel),
    mName(QString::fromStdWString(pModel->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::ImportUnits *pImportUnits) :
    CellmlFileElement(pImportUnits),
    mName(QString::fromStdWString(pImportUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileElement(pImportComponent),
    mName(QString::fromStdWString(pImportComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::Units *pUnits) :
    CellmlFileElement(pUnits),
    mName(QString::fromStdWString(pUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::Unit *pUnit) :
    CellmlFileElement(pUnit),
    mName(QString::fromStdWString(pUnit->units()))
{
}

//==============================================================================

QString CellmlFileNamedElement::name() const
{
    // Return the named element's name

    return mName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
