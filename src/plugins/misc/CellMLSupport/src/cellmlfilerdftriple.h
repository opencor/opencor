//==============================================================================
// CellML file RDF triple
//==============================================================================

#ifndef CELLMLFILERDFTRIPLE_H
#define CELLMLFILERDFTRIPLE_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QMap>
#include <QString>

//==============================================================================

#include "IfaceRDF_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriple
{
public:
    enum Type
    {
        Uri,
        ObjId,
        PlainLiteral,
        TypedLiteral
    };

    explicit CellmlFileRdfTriple(iface::rdf_api::Triple *pTriple);

    Type subjectType() const;
    QString subject() const;

    Type predicateType() const;
    QString predicate() const;

    Type objectType() const;
    QString object() const;
    QString objectExtra() const;

private:
    Type mSubjectType;
    QString mSubject;

    Type mPredicateType;
    QString mPredicate;

    Type mObjectType;
    QString mObject;
    QString mObjectExtra;
};

//==============================================================================

typedef QMap<QString, CellmlFileRdfTriple *> CellmlFileRdfTriples;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
