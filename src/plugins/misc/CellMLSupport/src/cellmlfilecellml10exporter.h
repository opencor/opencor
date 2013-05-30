//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#ifndef CELLMLFILECELLML10EXPORTER_H
#define CELLMLFILECELLML10EXPORTER_H

//==============================================================================

#include "cellmlfileexporter.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceAnnoTools.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileCellml10Exporter : public CellmlFileExporter
{
public:
    explicit CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                        const QString &pFileName);

private:
    ObjRef<iface::cellml_services::AnnotationSet> mAnnotationSet;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
