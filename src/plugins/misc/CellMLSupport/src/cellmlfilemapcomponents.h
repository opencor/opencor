//==============================================================================
// CellML file map components
//==============================================================================

#ifndef CELLMLFILEMAPCOMPONENTS_H
#define CELLMLFILEMAPCOMPONENTS_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileMapComponents : public CellmlFileElement
{
public:
    explicit CellmlFileMapComponents(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapComponents *pMapComponents);

    QString firstComponent() const;
    QString secondComponent() const;

private:
    QString mFirstComponent;
    QString mSecondComponent;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
