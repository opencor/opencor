//==============================================================================
// CellML file named element
//==============================================================================

#include "cellmlfilenamedelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileNamedElement::CellmlFileNamedElement(const QString &pCmetaId,
                                               const QString &pName) :
    CellmlFileElement(pCmetaId),
    mName(pName)
{
}

//==============================================================================

QString CellmlFileNamedElement::name() const
{
    // Return the named element's name

    return mName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
