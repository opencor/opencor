//==============================================================================
// CellML file named element
//==============================================================================

#include "cellmlfilenamedelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Model *pCellmlApiModel) :
    CellmlFileElement(pCellmlFile, pCellmlApiModel),
    mName(QString::fromStdWString(pCellmlApiModel->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::ImportUnits *pCellmlApiImportUnits) :
    CellmlFileElement(pCellmlFile, pCellmlApiImportUnits),
    mName(QString::fromStdWString(pCellmlApiImportUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::ImportComponent *pCellmlApiImportComponent) :
    CellmlFileElement(pCellmlFile, pCellmlApiImportComponent),
    mName(QString::fromStdWString(pCellmlApiImportComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Units *pCellmlApiUnits) :
    CellmlFileElement(pCellmlFile, pCellmlApiUnits),
    mName(QString::fromStdWString(pCellmlApiUnits->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::Unit *pCellmlApiUnit) :
    CellmlFileElement(pCellmlFile, pCellmlApiUnit),
    mName(QString::fromStdWString(pCellmlApiUnit->units()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::CellMLComponent *pCellmlApiComponent) :
    CellmlFileElement(pCellmlFile, pCellmlApiComponent),
    mName(QString::fromStdWString(pCellmlApiComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                               iface::cellml_api::CellMLVariable *pCellmlApiVariable) :
    CellmlFileElement(pCellmlFile, pCellmlApiVariable),
    mName(QString::fromStdWString(pCellmlApiVariable->name()))
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
