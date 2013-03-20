//==============================================================================
// CellML file class
//==============================================================================

#ifndef CELLMLFILE_H
#define CELLMLFILE_H

//==============================================================================

#include "cellmlfileissue.h"
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

    iface::cellml_api::Model * model() const;

    iface::rdf_api::DataSource * rdfDataSource() const;

    bool load();
    bool reload();

    bool save(const QString &pNewFileName = QString());

    bool isValid();

    bool isModified() const;
    void setModified(const bool &pModified) const;

    CellmlFileIssues issues() const;

    CellmlFileRuntime * runtime();

    QString fileName() const;

    CellmlFileRdfTriples * rdfTriples();

    QString uriBase() const;

    CellmlFileRdfTriples rdfTriples(const QString &pCmetaId) const;

private:
    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    ObjRef<iface::rdf_api::RDFAPIRepresentation> mRdfApiRepresentation;
    ObjRef<iface::rdf_api::DataSource> mRdfDataSource;

    QString mUriBase;

    CellmlFileRdfTriples mRdfTriples;

    bool mValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mValidNeeded;
    bool mRuntimeUpdateNeeded;

    void reset();

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
