//==============================================================================
// CellML file import unit
//==============================================================================

#include "cellmlfileimportunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportUnit::CellmlFileImportUnit(CellmlFile *pCellmlFile,
                                           iface::cellml_api::ImportUnits *pCellmlApiImportUnits) :
    CellmlFileUnit(pCellmlFile, pCellmlApiImportUnits),
    mUnitReference(QString::fromStdWString(pCellmlApiImportUnits->unitsRef()))
{
}

//==============================================================================

QString CellmlFileImportUnit::unitReference() const
{
    // Return the import unit's reference name

    return mUnitReference;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
