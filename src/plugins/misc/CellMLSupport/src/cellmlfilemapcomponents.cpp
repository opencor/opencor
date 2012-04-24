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
    mFirstComponentName(QString::fromStdWString(pMapComponents->firstComponentName())),
    mSecondComponentName(QString::fromStdWString(pMapComponents->secondComponentName()))
{
}

//==============================================================================

QString CellmlFileMapComponents::firstComponentName() const
{
    // Return the map components' first component's name

    return mFirstComponentName;
}

//==============================================================================

QString CellmlFileMapComponents::secondComponentName() const
{
    // Return the map components' second component's name

    return mSecondComponentName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
