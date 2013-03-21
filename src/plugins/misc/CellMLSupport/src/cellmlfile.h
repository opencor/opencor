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

    CellmlFileRdfTriples & rdfTriples();
    CellmlFileRdfTriples rdfTriples(iface::cellml_api::CellMLElement *pElement) const;

    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                         const QString &pResource, const QString &pId) const;
    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                         const QString &pResource, const QString &pId) const;

    CellMLSupport::CellmlFileRdfTriple * addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                      const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                      const QString &pResource,
                                                      const QString &pId);
    CellMLSupport::CellmlFileRdfTriple * addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                      const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                      const QString &pResource,
                                                      const QString &pId);

    QString uriBase() const;

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

    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const QString &pQualifier,
                         const QString &pResource, const QString &pId) const;

    QString rdfTripleSubject(iface::cellml_api::CellMLElement *pElement) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
