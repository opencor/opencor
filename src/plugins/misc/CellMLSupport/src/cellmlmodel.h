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

#include "IfaceCCGS.hxx"
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

static const QString Cellml_1_0 = "1.0";
static const QString Cellml_1_1 = "1.1";

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
    ObjRef<iface::cellml_services::CodeInformation> mCodeInformation;

    QString mErrorMessage;

    void resetProperties();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
