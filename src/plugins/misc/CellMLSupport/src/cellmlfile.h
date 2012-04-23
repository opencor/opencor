//==============================================================================
// CellML file class
//==============================================================================

#ifndef CELLMLFILE_H
#define CELLMLFILE_H

//==============================================================================

#include "cellmlfilegroup.h"
#include "cellmlfileimport.h"
#include "cellmlfileissue.h"
#include "cellmlfilemodel.h"
#include "cellmlfileruntime.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFile : public QObject
{
    Q_OBJECT

public:
    explicit CellmlFile(const QString &pFileName);
    ~CellmlFile();

    bool load();
    bool reload();

    bool isValid();

    CellmlFileIssues issues() const;

    CellmlFileRuntime * runtime();

    CellmlFileModel * model() const;

    CellmlFileImports imports() const;
    CellmlFileUnits units() const;
    CellmlFileGroups groups() const;

private:
    QString mFileName;

    ObjRef<iface::cellml_api::Model> mCellmlApiModel;

    CellmlFileModel *mModel;
    CellmlFileImports mImports;
    CellmlFileUnits mUnits;
    CellmlFileGroups mGroups;

    bool mIsValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mIsValidNeeded;
    bool mRuntimeUpdateNeeded;

    void reset();

    void clearImports();
    void clearUnits();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
