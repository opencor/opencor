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

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriple
{
public:
    enum Type {
        Unknown,
        ModelQualifier,
        BiologyQualifier
    };

    enum ModelQualifierType {
        ModelUnknown,
        ModelIs,
        ModelIsDerivedFrom,
        ModelIsDescribedBy
    };

    enum BiologyQualifierType {
        BiologyUnknown,
        BiologyEncodes,
        BiologyHasPart,
        BiologyHasProperty,
        BiologyHasVersion,
        BiologyIs,
        BiologyIsDescribedBy,
        BiologyIsEncodedBy,
        BiologyIsHomologTo,
        BiologyIsPartOf,
        BiologyIsPropertyOf,
        BiologyIsVersionOf,
        BiologyOccursIn,
        BiologyHasTaxon
    };

    explicit CellmlFileRdfTriple(iface::rdf_api::Triple *pRdfTriple);
    ~CellmlFileRdfTriple();

    CellmlFileRdfTripleElement * subject() const;
    CellmlFileRdfTripleElement * predicate() const;
    CellmlFileRdfTripleElement * object() const;

    Type type() const;

    ModelQualifierType modelQualifierType() const;
    QString modelQualifierTypeAsString() const;

    BiologyQualifierType biologyQualifierType() const;
    QString biologyQualifierTypeAsString() const;

    QUrl qualifierUrl() const;

private:
    CellmlFileRdfTripleElement *mSubject;
    CellmlFileRdfTripleElement *mPredicate;
    CellmlFileRdfTripleElement *mObject;

    Type mType;

    ModelQualifierType mModelQualifierType;
    BiologyQualifierType mBiologyQualifierType;

    QUrl mQualifierUrl;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriples: public QList<CellmlFileRdfTriple *>
{
public:
    CellmlFileRdfTriple::Type type() const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
