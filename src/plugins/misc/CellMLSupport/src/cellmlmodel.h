//==============================================================================
// CellML model class
//==============================================================================

#ifndef CELLMLMODEL_H
#define CELLMLMODEL_H

//==============================================================================

#include "cellmlmodelissue.h"
#include "cellmlmodelruntime.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QObject>
#include <QStringList>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModel : public QObject
{
    Q_OBJECT

public:
    explicit CellmlModel(const QString &pFileName);
    ~CellmlModel();

    bool load();
    bool reload();

    bool isValid();

    QList<CellmlModelIssue> issues();

    CellmlModelRuntime * runtime();

private:
    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    QList<CellmlModelIssue> mIssues;

    CellmlModelRuntime *mRuntime;

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
