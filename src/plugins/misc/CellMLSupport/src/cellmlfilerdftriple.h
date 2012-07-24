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

class CellmlFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriple
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

    explicit CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                 iface::rdf_api::Triple *pRdfTriple);
    explicit CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                 const QString pSubject,
                                 const ModelQualifier &pModelQualifier,
                                 const QString &pResource, const QString &pId);
    explicit CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                 const QString pSubject,
                                 const BioQualifier &pBioQualifier,
                                 const QString &pResource, const QString &pId);
    ~CellmlFileRdfTriple();

    CellmlFileRdfTripleElement * subject() const;
    CellmlFileRdfTripleElement * predicate() const;
    CellmlFileRdfTripleElement * object() const;

    Type type() const;

    QString metadataId() const;

    ModelQualifier modelQualifier() const;
    QString modelQualifierAsString() const;
    static QString modelQualifierAsString(const ModelQualifier &pModelQualifier);

    BioQualifier bioQualifier() const;
    QString bioQualifierAsString() const;
    static QString bioQualifierAsString(const BioQualifier &pBioQualifier);

    static QStringList qualifiersAsStringList();

    QString resource() const;
    QString id() const;

    static void decodeMiriamUrn(const QString &pMiriamUrn, QString &pResource, QString &pId);
    static void decodeIdentifiersDotOrgUri(const QString &pIdentifiersDotOrgUri, QString &pResource, QString &pId);

private:
    CellmlFile *mCellmlFile;

    CellmlFileRdfTripleElement *mSubject;
    CellmlFileRdfTripleElement *mPredicate;
    CellmlFileRdfTripleElement *mObject;

    Type mType;

    ModelQualifier mModelQualifier;
    BioQualifier mBioQualifier;

    QString mResource;
    QString mId;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriples: public QList<CellmlFileRdfTriple *>
{
public:
    explicit CellmlFileRdfTriples(CellmlFile *pCellmlFile = 0);

    CellmlFileRdfTriple::Type type() const;

    CellmlFileRdfTriples contains(const QString &pMetadataId) const;

    CellmlFileRdfTriple * add(CellmlFileRdfTriple *pRdfTriple);

    void remove(CellmlFileRdfTriple *pRdfTriple);
    void remove(const QString &pMetadataId);
    void removeAll();

private:
    CellmlFile *mCellmlFile;

    void recursiveContains(CellmlFileRdfTriples &pRdfTriples,
                           CellmlFileRdfTriple *pRdfTriple) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
