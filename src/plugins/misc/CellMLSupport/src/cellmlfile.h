//==============================================================================
// CellML file class
//==============================================================================

#ifndef CELLMLFILE_H
#define CELLMLFILE_H

//==============================================================================

#include "cellmlfileimport.h"
#include "cellmlfileissue.h"
#include "cellmlfileruntime.h"
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

    QString modelName() const;

    CellmlFileImports imports() const;

private:
    QString mFileName;

    iface::cellml_api::Model *mModel;

    bool mIsValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mIsValidNeeded;
    bool mRuntimeUpdateNeeded;

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
