//==============================================================================
// CellML file named element
//==============================================================================

#ifndef CELLMLFILENAMEDELEMENT_H
#define CELLMLFILENAMEDELEMENT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileNamedElement : public CellmlFileElement
{
public:
    explicit CellmlFileNamedElement(const QString &pCmetaId,
                                    const QString &pName);

    QString name() const;

private:
    QString mName;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
