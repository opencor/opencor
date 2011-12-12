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

    explicit CellmlModelIssue(const Type &pType, const QString &pMessage,
                              const uint32_t &pLine = Undefined,
                              const uint32_t &pColumn = Undefined,
                              const QString &pImportedModel = QString());

    Type type() const;
    QString message() const;
    uint32_t line() const;
    uint32_t column() const;
    QString importedModel() const;

private:
    Type mType;
    QString mMessage;
    uint32_t mLine;
    uint32_t mColumn;
    QString mImportedModel;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelRuntime
{
    friend class CellmlModel;

private:
    void reset();
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

    CellmlModelRuntime runtime();

private:
    ObjRef<iface::cellml_api::CellMLBootstrap> mCellmlBootstrap;
    ObjRef<iface::cellml_api::DOMModelLoader> mModelLoader;

    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    QList<CellmlModelIssue> mIssues;

    CellmlModelRuntime mRuntime;

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
