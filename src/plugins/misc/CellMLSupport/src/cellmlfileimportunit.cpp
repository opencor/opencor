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
    mReferenceName(QString::fromStdWString(pImportUnits->unitsRef()))
{
}

//==============================================================================

QString CellmlFileImportUnit::referenceName() const
{
    // Return the import unit's reference name

    return mReferenceName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
