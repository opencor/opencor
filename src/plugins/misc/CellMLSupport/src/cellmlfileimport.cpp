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

    ObjRef<iface::cellml_api::ImportUnitsSet> importUnits = pCellmlImport->units();
    ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitIterator = importUnits->iterateImportUnits();

    while (true) {
        ObjRef<iface::cellml_api::ImportUnits> importUnit = importUnitIterator->nextImportUnits();

        if (importUnit)
            // We have a unit import, so add it to our list

            mUnits.append(new CellmlFileImportUnit(importUnit));
        else
            // No more unit imports, so...

            break;
    }

    // ... and of any component imports

    ObjRef<iface::cellml_api::ImportComponentSet> importComponents = pCellmlImport->components();
    ObjRef<iface::cellml_api::ImportComponentIterator> importComponentIterator = importComponents->iterateImportComponents();

    while (true) {
        ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentIterator->nextImportComponent();

        if (importComponent)
            // We have a component import, so add it to our list

            mComponents.append(new CellmlFileImportComponent(importComponent));
        else
            // No more component imports, so...

            break;
    }
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
