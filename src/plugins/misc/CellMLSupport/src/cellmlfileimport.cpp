//==============================================================================
// CellML file import
//==============================================================================

#include "cellmlfileimport.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImport::CellmlFileImport(iface::cellml_api::CellMLImport *pCellmlImport) :
    CellmlFileElement(pCellmlImport),
    mUri(QString::fromStdWString(pCellmlImport->xlinkHref()->asText())),
    mUnits(CellmlFileImportUnits()),
    mComponents(CellmlFileImportComponents())
{
    // Keep track of any unit imports...

    iface::cellml_api::ImportUnitsIterator *importUnitsIterator = pCellmlImport->units()->iterateImportUnits();
    iface::cellml_api::ImportUnits *importUnits;

    while ((importUnits = importUnitsIterator->nextImportUnits()))
        mUnits.append(new CellmlFileImportUnit(importUnits));

    // ... and of any component imports

    iface::cellml_api::ImportComponentIterator *importComponentIterator = pCellmlImport->components()->iterateImportComponents();
    iface::cellml_api::ImportComponent *importComponent;

    while ((importComponent = importComponentIterator->nextImportComponent()))
        mComponents.append(new CellmlFileImportComponent(importComponent));
}

//==============================================================================

CellmlFileImport::~CellmlFileImport()
{
    // Delete some internal objects

    foreach (CellmlFileImportUnit *unit, mUnits)
        delete unit;

    foreach (CellmlFileImportComponent *component, mComponents)
        delete component;
}

//==============================================================================

QString CellmlFileImport::uri() const
{
    // Return the import's URI

    return mUri;
}

//==============================================================================

CellmlFileImportUnits CellmlFileImport::units() const
{
    // Return the import's units

    return mUnits;
}

//==============================================================================

CellmlFileImportComponents CellmlFileImport::components() const
{
    // Return the import's components

    return mComponents;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
