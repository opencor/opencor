//==============================================================================
// CellML model runtime class
//==============================================================================

#ifndef CELLMLMODELRUNTIME_H
#define CELLMLMODELRUNTIME_H

//==============================================================================

#include "cellmlmodelcommon.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelRuntime : public QObject
{
    Q_OBJECT

public:
    bool isValid();

    QList<CellmlModelIssue> issues();

    CellmlModelRuntime * update(iface::cellml_api::Model *pModel,
                                const bool &pValidModel);

private:
    void reset();

    QList<CellmlModelIssue> mIssues;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
