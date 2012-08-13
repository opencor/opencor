//==============================================================================
// CellML file import
//==============================================================================

#include "cellmlfileimport.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImport::CellmlFileImport(CellmlFile *pCellmlFile,
                                   iface::cellml_api::CellMLImport *pCellmlApiImport) :
    CellmlFileElement(pCellmlFile, pCellmlApiImport),
    mXlinkHref(QString::fromStdWString(pCellmlApiImport->xlinkHref()->asText())),
    mUnits(CellmlFileImportUnits()),
    mComponents(CellmlFileImportComponents())
{
    // Keep track of any unit imports...

    ObjRef<iface::cellml_api::ImportUnitsSet> importUnits = pCellmlApiImport->units();
    ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitIterator = importUnits->iterateImportUnits();

    forever {
        iface::cellml_api::ImportUnits *importUnit = importUnitIterator->nextImportUnits();

        if (!importUnit)
            break;

        mUnits << new CellmlFileImportUnit(pCellmlFile, importUnit);
    }

    // ... and of any component imports

    ObjRef<iface::cellml_api::ImportComponentSet> importComponents = pCellmlApiImport->components();
    ObjRef<iface::cellml_api::ImportComponentIterator> importComponentIterator = importComponents->iterateImportComponents();

    forever {
        iface::cellml_api::ImportComponent *importComponent = importComponentIterator->nextImportComponent();

        if (!importComponent)
            break;

        mComponents << new CellmlFileImportComponent(pCellmlFile, importComponent);
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
