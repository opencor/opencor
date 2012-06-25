//==============================================================================
// CellML file import
//==============================================================================

#include "cellmlfileimport.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImport::CellmlFileImport(CellmlFile *pCellmlFile,
                                   iface::cellml_api::CellMLImport *pCellmlImport) :
    CellmlFileElement(pCellmlFile, pCellmlImport),
    mXlinkHref(QString::fromStdWString(pCellmlImport->xlinkHref()->asText())),
    mUnits(CellmlFileImportUnits()),
    mComponents(CellmlFileImportComponents())
{
    // Keep track of any unit imports...

    ObjRef<iface::cellml_api::ImportUnitsSet> importUnits = pCellmlImport->units();
    ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitIterator = importUnits->iterateImportUnits();

    forever {
        ObjRef<iface::cellml_api::ImportUnits> importUnit = importUnitIterator->nextImportUnits();

        if (importUnit)
            // We have a unit import, so add it to our list

            mUnits << new CellmlFileImportUnit(pCellmlFile, importUnit);
        else
            // No more unit imports, so...

            break;
    }

    // ... and of any component imports

    ObjRef<iface::cellml_api::ImportComponentSet> importComponents = pCellmlImport->components();
    ObjRef<iface::cellml_api::ImportComponentIterator> importComponentIterator = importComponents->iterateImportComponents();

    forever {
        ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentIterator->nextImportComponent();

        if (importComponent)
            // We have a component import, so add it to our list

            mComponents << new CellmlFileImportComponent(pCellmlFile, importComponent);
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

QString CellmlFileImport::xlinkHref() const
{
    // Return the import's xlink:href

    return mXlinkHref;
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
