//==============================================================================
// CellML file RDF triple
//==============================================================================

#include "cellmlfilerdftriple.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(iface::rdf_api::Triple *pTriple)
{
    // Retrieve the triple's subject, predicate and object information

    ObjRef<iface::rdf_api::Resource> subject = pTriple->subject();
    ObjRef<iface::rdf_api::Resource> predicate = pTriple->predicate();
    ObjRef<iface::rdf_api::Node> object = pTriple->object();

    mSubject   = new CellmlFileRdfTripleElement(subject);
    mPredicate = new CellmlFileRdfTripleElement(predicate);
    mObject    = new CellmlFileRdfTripleElement(object);
}

//==============================================================================

CellmlFileRdfTriple::~CellmlFileRdfTriple()
{
    // Delete some internal objects

    delete mSubject;
    delete mPredicate;
    delete mObject;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::subject() const
{
    // Return the RDF triple's subject

    return mSubject;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::predicate() const
{
    // Return the RDF triple's predicate

    return mPredicate;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::object() const
{
    // Return the RDF triple's object

    return mObject;
}

//==============================================================================

CellmlFileRdfTriple::Type rdfTripleType(CellmlFileRdfTriple *pRdfTriple)
{
    // Return the type of the RDF triple

    return CellmlFileRdfTriple::Unknown;
}

//==============================================================================

CellmlFileRdfTriple::Type rdfTriplesType(const CellmlFileRdfTriples &pRdfTriples)
{
    // Return the type of the RDF triples
    // Note: if the RDF triples are of different types each then the overall
    //       type is considered to be unknown

    if (pRdfTriples.isEmpty()) {
        // There are no RDF triples, so...

        return CellmlFileRdfTriple::Unknown;
    } else {
        // There is at least one RDF triple, so use the type of the first RDF
        // triple as the default overall type

        CellmlFileRdfTriple::Type res = rdfTripleType(pRdfTriples.first());

        // Go through the different RDF triples and make sure that their type
        // is consistent

        foreach (CellmlFileRdfTriple *rdfTriple, pRdfTriples)
            if (rdfTripleType(rdfTriple) != res)
                // The current RDF triple has a different type from the others,
                // so...

                return CellmlFileRdfTriple::Unknown;

        // All of the RDF triples have the same type, so...

        return res;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
