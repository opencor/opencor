//==============================================================================
// CellML file unit element
//==============================================================================

#ifndef CELLMLFILEUNITELEMENT_H
#define CELLMLFILEUNITELEMENT_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileUnitElement : public CellmlFileNamedElement
{
public:
    explicit CellmlFileUnitElement(CellmlFile *pCellmlFile,
                                   iface::cellml_api::Unit *pUnit);

    int prefix() const;
    double multiplier() const;
    double offset() const;
    double exponent() const;

private:
    int mPrefix;
    double mMultiplier;
    double mOffset;
    double mExponent;
};

//==============================================================================

typedef QList<CellmlFileUnitElement *> CellmlFileUnitElements;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
