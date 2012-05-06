//==============================================================================
// CellML file map components
//==============================================================================

#include "cellmlfilemapcomponents.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileMapComponents::CellmlFileMapComponents(iface::cellml_api::MapComponents *pMapComponents) :
    CellmlFileElement(pMapComponents),
    mFirstComponent(QString::fromStdWString(pMapComponents->firstComponentName())),
    mSecondComponent(QString::fromStdWString(pMapComponents->secondComponentName()))
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
