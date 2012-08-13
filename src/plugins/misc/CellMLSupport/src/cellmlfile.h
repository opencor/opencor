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

    iface::cellml_api::Model * cellmlApiModel() const;

    bool load();
    bool reload();

    bool save(const QString &pNewFileName = QString());

    bool isValid();

    bool isModified() const;
    void setModified(const bool &pModified) const;

    CellmlFileIssues issues() const;

    CellmlFileRuntime * runtime();

    QString fileName() const;

    CellmlFileModel * model() const;

    CellmlFileImports * imports();
    CellmlFileUnits * units();
    CellmlFileComponents * components();
    CellmlFileGroups * groups();
    CellmlFileConnections * connections();
    CellmlFileRdfTriples * rdfTriples();

    QString uriBase() const;

    CellmlFileRdfTriples rdfTriples(const QString &pCmetaId) const;

private:
    QString mFileName;

    iface::cellml_api::Model *mCellmlApiModel;

    iface::rdf_api::RDFAPIRepresentation *mCellmlApiRdfApiRepresentation;
    iface::rdf_api::DataSource *mCellmlApiRdfDataSource;

    QString mUriBase;

    CellmlFileModel *mModel;
    CellmlFileImports mImports;
    CellmlFileUnits mUnits;
    CellmlFileComponents mComponents;
    CellmlFileGroups mGroups;
    CellmlFileConnections mConnections;
    CellmlFileRdfTriples mRdfTriples;

    bool mValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mValidNeeded;
    bool mRuntimeUpdateNeeded;

    void reset();

    void clearImports();
    void clearUnits();
    void clearComponents();
    void clearGroups();
    void clearConnections();
    void clearRdfTriples();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
