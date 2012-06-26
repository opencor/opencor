//==============================================================================
// CellML file named element
//==============================================================================

#include "cellmlfilenamedelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Model *pModel) :
    CellmlFileElement(pCellmlFile, pModel),
    mName(QString::fromStdWString(pModel->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::ImportUnits *pImportUnits) :
    CellmlFileElement(pCellmlFile, pImportUnits),
    mName(QString::fromStdWString(pImportUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileElement(pCellmlFile, pImportComponent),
    mName(QString::fromStdWString(pImportComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Units *pUnits) :
    CellmlFileElement(pCellmlFile, pUnits),
    mName(QString::fromStdWString(pUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Unit *pUnit) :
    CellmlFileElement(pCellmlFile, pUnit),
    mName(QString::fromStdWString(pUnit->units()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileElement(pCellmlFile, pComponent),
    mName(QString::fromStdWString(pComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::CellMLVariable *pVariable) :
    CellmlFileElement(pCellmlFile, pVariable),
    mName(QString::fromStdWString(pVariable->name()))
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
