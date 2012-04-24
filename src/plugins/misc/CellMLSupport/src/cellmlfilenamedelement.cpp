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

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileElement(pComponent),
    mName(QString::fromStdWString(pComponent->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::CellMLVariable *pVariable) :
    CellmlFileElement(pVariable),
    mName(QString::fromStdWString(pVariable->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::RelationshipRef *pRelationshipRef) :
    CellmlFileElement(pRelationshipRef),
    mName(QString::fromStdWString(pRelationshipRef->name()))
{
}

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(iface::cellml_api::ComponentRef *pComponentRef) :
    CellmlFileElement(pComponentRef),
    mName(QString::fromStdWString(pComponentRef->componentName()))
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
