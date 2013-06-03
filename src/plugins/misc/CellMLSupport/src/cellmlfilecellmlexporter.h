//==============================================================================
// CellML file CellML exporter
//==============================================================================

#ifndef CELLMLFILECELLMLEXPORTER_H
#define CELLMLFILECELLMLEXPORTER_H

//==============================================================================

#include "cellmlfileexporter.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

static const std::wstring MATHML_NAMESPACE = L"http://www.w3.org/1998/Math/MathML";

static const std::wstring CELLML_1_0_NAMESPACE = L"http://www.cellml.org/cellml/1.0#";
static const std::wstring CELLML_1_1_NAMESPACE = L"http://www.cellml.org/cellml/1.1#";

//==============================================================================

class CellmlFileCellmlExporter : public CellmlFileExporter
{
public:
    explicit CellmlFileCellmlExporter(iface::cellml_api::Model *pModel,
                                      const std::wstring &pVersion);

protected:
    iface::cellml_api::Model *mModel;
    ObjRef<iface::cellml_api::Model> mExportedModel;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
