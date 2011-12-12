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

static const uint32_t Undefined = 0;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelIssue
{
public:
    enum Type
    {
        Error,
        Warning
    };

    explicit CellmlModelIssue(const Type &pType, const uint32_t &pLine,
                              const uint32_t &pColumn, const QString &pMessage);

    Type type() const;
    uint32_t line() const;
    uint32_t column() const;
    QString message() const;

private:
    Type mType;
    uint32_t mLine;
    uint32_t mColumn;
    QString mMessage;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModel
{
public:
    explicit CellmlModel(const QString &pFileName);

    bool load();
    bool reload();

    bool isValid();

    QList<CellmlModelIssue> issues();

private:
    ObjRef<iface::cellml_api::CellMLBootstrap> mCellmlBootstrap;
    ObjRef<iface::cellml_api::DOMModelLoader> mModelLoader;

    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    QList<CellmlModelIssue> mIssues;

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
