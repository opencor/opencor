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

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTriple
{
public:
    enum Type {
        Unknown
    };

    explicit CellmlFileRdfTriple(iface::rdf_api::Triple *pTriple);
    ~CellmlFileRdfTriple();

    CellmlFileRdfTripleElement * subject() const;
    CellmlFileRdfTripleElement * predicate() const;
    CellmlFileRdfTripleElement * object() const;

private:
    CellmlFileRdfTripleElement *mSubject;
    CellmlFileRdfTripleElement *mPredicate;
    CellmlFileRdfTripleElement *mObject;
};

//==============================================================================

typedef QList<CellmlFileRdfTriple *> CellmlFileRdfTriples;

//==============================================================================

CellmlFileRdfTriple::Type CELLMLSUPPORT_EXPORT rdfTripleType(CellmlFileRdfTriple *pRdfTriple);
CellmlFileRdfTriple::Type CELLMLSUPPORT_EXPORT rdfTriplesType(const CellmlFileRdfTriples &pRdfTriples);

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
