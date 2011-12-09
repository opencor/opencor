//==============================================================================
// CellML model class
//==============================================================================

#ifndef CELLMLMODEL_H
#define CELLMLMODEL_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QStringList>

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

static const QString Cellml_1_0 = "1.0";
static const QString Cellml_1_1 = "1.1";

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModel
{
public:
    explicit CellmlModel(const QString &pFileName);

    bool load();
    bool reload();

    bool isValid();

    QStringList errorMessages();

private:
    ObjRef<iface::cellml_api::CellMLBootstrap> mCellmlBootstrap;
    ObjRef<iface::cellml_api::DOMModelLoader> mModelLoader;

    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    QStringList mErrorMessages;

    void reset();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
