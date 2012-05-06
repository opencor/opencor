//==============================================================================
// CellML file import unit
//==============================================================================

#include "cellmlfileimportunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportUnit::CellmlFileImportUnit(iface::cellml_api::ImportUnits *pImportUnits) :
    CellmlFileUnit(pImportUnits),
    mUnitReference(QString::fromStdWString(pImportUnits->unitsRef()))
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
