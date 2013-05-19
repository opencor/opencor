//==============================================================================
// CellML file RDF triple
//==============================================================================

#ifndef CELLMLFILERDFTRIPLE_H
#define CELLMLFILERDFTRIPLE_H

//==============================================================================

#include "cellmlfilerdftripleelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QSet>
#include <QUrl>
#include <QVector>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

static const QString ResourceRegExp = "[0-9a-z]+((-|\\.)[0-9a-z]+)?";
static const QString IdRegExp = "[0-9A-Za-z\\.%-_:]+(:[0-9A-Za-z\\.%-_:]+)?";

//==============================================================================

class CellMLFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileRdfTriple
{
public:
    enum Type {
        Unknown,
        BioModelsDotNetQualifier,
        Empty
    };

    enum ModelQualifier {
        ModelUnknown,
        ModelIs,
        ModelIsDerivedFrom,
        ModelIsDescribedBy,
        FirstModelQualifier = ModelIs,
        LastModelQualifier = ModelIsDescribedBy
    };

    enum BioQualifier {
        BioUnknown,
        BioEncodes,
        BioHasPart,
        BioHasProperty,
        BioHasVersion,
        BioIs,
        BioIsDescribedBy,
        BioIsEncodedBy,
        BioIsHomologTo,
        BioIsPartOf,
        BioIsPropertyOf,
        BioIsVersionOf,
        BioOccursIn,
        BioHasTaxon,
        FirstBioQualifier = BioEncodes,
        LastBioQualifier = BioHasTaxon
    };

    explicit CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                 iface::rdf_api::Triple *pRdfTriple);
    explicit CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                 const QString pSubject,
                                 const ModelQualifier &pModelQualifier,
                                 const QString &pResource, const QString &pId);
    explicit CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                 const QString pSubject,
                                 const BioQualifier &pBioQualifier,
                                 const QString &pResource, const QString &pId);
    ~CellMLFileRdfTriple();

    iface::rdf_api::Triple * rdfTriple() const;
    void setRdfTriple(iface::rdf_api::Triple *pRdfTriple);

    CellMLFileRdfTripleElement * subject() const;
    CellMLFileRdfTripleElement * predicate() const;
    CellMLFileRdfTripleElement * object() const;

    Type type() const;

    QString metadataId() const;

    QString qualifierAsString() const;

    ModelQualifier modelQualifier() const;
    QString modelQualifierAsString() const;
    static QString modelQualifierAsString(const ModelQualifier &pModelQualifier);

    BioQualifier bioQualifier() const;
    QString bioQualifierAsString() const;
    static QString bioQualifierAsString(const BioQualifier &pBioQualifier);

    static QStringList qualifiersAsStringList();

    QString resource() const;
    QString id() const;

    static bool decodeTerm(const QString &pTerm, QString &pResource, QString &pId);

private:
    CellMLFile *mCellMLFile;

    ObjRef<iface::rdf_api::Triple> mRdfTriple;

    CellMLFileRdfTripleElement *mSubject;
    CellMLFileRdfTripleElement *mPredicate;
    CellMLFileRdfTripleElement *mObject;

    Type mType;

    ModelQualifier mModelQualifier;
    BioQualifier mBioQualifier;

    QString mResource;
    QString mId;

    void constructor(CellMLFile *pCellMLFile,
                     iface::rdf_api::Triple *pRdfTriple, const Type &pType,
                     const ModelQualifier &pModelQualifier,
                     const BioQualifier &pBioQualifier,
                     const QString &pResource, const QString &pId);
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileRdfTriples: public QList<CellMLFileRdfTriple *>
{
public:
    explicit CellMLFileRdfTriples(CellMLFile *pCellMLFile);

    CellMLFileRdfTriple::Type type() const;

    CellMLFileRdfTriples contains(iface::cellml_api::CellMLElement *pElement) const;

    CellMLFileRdfTriple * add(CellMLFileRdfTriple *pRdfTriple);

    void remove(CellMLFileRdfTriple *pRdfTriple);
    void remove(iface::cellml_api::CellMLElement *pElement);
    void removeAll();

private:
    CellMLFile *mCellMLFile;

    void recursiveContains(CellMLFileRdfTriples &pRdfTriples,
                           CellMLFileRdfTriple *pRdfTriple) const;

    void removeRdfTriples(const CellMLFileRdfTriples &pRdfTriples);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
