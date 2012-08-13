//==============================================================================
// CellML file component
//==============================================================================

#ifndef CELLMLFILECOMPONENT_H
#define CELLMLFILECOMPONENT_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlfileunit.h"
#include "cellmlfilemathmlelement.h"
#include "cellmlfilevariable.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponent : public CellmlFileNamedElement
{
public:
    explicit CellmlFileComponent(CellmlFile *pCellmlFile,
                                 iface::cellml_api::ImportComponent *pCellmlApiImportComponent);
    explicit CellmlFileComponent(CellmlFile *pCellmlFile,
                                 iface::cellml_api::CellMLComponent *pCellmlApiComponent);
    ~CellmlFileComponent();

    CellmlFileUnits * units();
    CellmlFileVariables * variables();
    CellmlFileMathmlElements * mathmlElements();

private:
    CellmlFileUnits mUnits;
    CellmlFileVariables mVariables;
    CellmlFileMathmlElements mMathmlElements;
};

//==============================================================================

typedef QList<CellmlFileComponent *> CellmlFileComponents;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
