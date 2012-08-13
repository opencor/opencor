//==============================================================================
// CellML file map components
//==============================================================================

#include "cellmlfilemapcomponents.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileMapComponents::CellmlFileMapComponents(CellmlFile *pCellmlFile,
                                                 iface::cellml_api::MapComponents *pCellmlApiMapComponents) :
    CellmlFileElement(pCellmlFile, pCellmlApiMapComponents),
    mFirstComponent(QString::fromStdWString(pCellmlApiMapComponents->firstComponentName())),
    mSecondComponent(QString::fromStdWString(pCellmlApiMapComponents->secondComponentName()))
{
}

//==============================================================================

QString CellmlFileMapComponents::firstComponent() const
{
    // Return the map components' first component

    return mFirstComponent;
}

//==============================================================================

QString CellmlFileMapComponents::secondComponent() const
{
    // Return the map components' second component

    return mSecondComponent;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
