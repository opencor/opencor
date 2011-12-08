//==============================================================================
// CellML model class
//==============================================================================

#ifndef CELLMLMODEL_H
#define CELLMLMODEL_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QUrl>

//==============================================================================

#include "cellml-api-cxx-support.hpp"
#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace iface {
namespace cellml_api {
    class Model;
}   // namespace cellml_api
}   // namespace iface

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModel
{
public:
    explicit CellmlModel(const QUrl &pUrl);

    bool isValid();
    QString errorMessage();

private:
    QUrl mUrl;

    ObjRef<iface::cellml_api::Model> mModel;

    QString mErrorMessage;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
