//==============================================================================
// CellML file class
//==============================================================================

#ifndef CELLMLFILE_H
#define CELLMLFILE_H

//==============================================================================

#include "cellmlfilecomponent.h"
#include "cellmlfileconnection.h"
#include "cellmlfilegroup.h"
#include "cellmlfileimport.h"
#include "cellmlfileissue.h"
#include "cellmlfilemodel.h"
#include "cellmlfilerdftriple.h"
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
    CellmlFileComponents components() const;
    CellmlFileGroups groups() const;
    CellmlFileConnections connections() const;
    CellmlFileRdfTriples metadata() const;

    QString uriBase() const;

private:
    QString mFileName;

    ObjRef<iface::cellml_api::Model> mCellmlApiModel;

    CellmlFileModel *mModel;
    CellmlFileImports mImports;
    CellmlFileUnits mUnits;
    CellmlFileComponents mComponents;
    CellmlFileGroups mGroups;
    CellmlFileConnections mConnections;
    CellmlFileRdfTriples mMetadata;

    bool mIsValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mIsValidNeeded;
    bool mRuntimeUpdateNeeded;

    void reset();

    void clearImports();
    void clearUnits();
    void clearComponents();
    void clearGroups();
    void clearConnections();
    void clearMetadata();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
