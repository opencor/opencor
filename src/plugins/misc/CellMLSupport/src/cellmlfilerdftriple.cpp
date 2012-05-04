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

qDebug("---------------------------------------");
qDebug("Subject: %s", qPrintable(mSubject->asString()));
qDebug("Predicate: %s", qPrintable(mPredicate->asString()));
qDebug("Object: %s", qPrintable(mObject->asString()));
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

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
