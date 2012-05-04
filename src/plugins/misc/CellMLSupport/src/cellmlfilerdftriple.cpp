//==============================================================================
// CellML file RDF triple
//==============================================================================

#include "cellmlfilerdftriple.h"

//==============================================================================

#include "cellml-api-cxx-support.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(iface::rdf_api::Triple *pTriple)
{
    // Retrieve the triple's subject information

    ObjRef<iface::rdf_api::Resource> subject = pTriple->subject();
    ObjRef<iface::rdf_api::URIReference> subjectUriReference;
    QUERY_INTERFACE(subjectUriReference, subject, rdf_api::URIReference);

qDebug("---------------------------------------");
qDebug("Subject - Supported interfaces:");
std::vector<std::string> supportedInterfaces = subject->supported_interfaces();
for (int i = 0, iMax = supportedInterfaces.size(); i < iMax; ++i)
    qDebug(" - %s", supportedInterfaces[i].c_str());

    if (subjectUriReference) {
        mSubjectType = Uri;
        mSubject     = QString::fromStdWString(subjectUriReference->URI());
    } else {
        mSubjectType = ObjId;
        mSubject     = QString::fromStdString(subject->objid());
    }

    // Retrieve the triple's predicate information

    ObjRef<iface::rdf_api::Resource> predicate = pTriple->predicate();
    ObjRef<iface::rdf_api::URIReference> predicateUriReference;
    QUERY_INTERFACE(predicateUriReference, predicate, rdf_api::URIReference);

qDebug("---------------------------------------");
qDebug("Predicate - Supported interfaces:");
supportedInterfaces = predicate->supported_interfaces();
for (int i = 0, iMax = supportedInterfaces.size(); i < iMax; ++i)
    qDebug(" - %s", supportedInterfaces[i].c_str());

    if (predicateUriReference) {
        mPredicateType = Uri;
        mPredicate     = QString::fromStdWString(predicateUriReference->URI());
    } else {
        mPredicateType = ObjId;
        mPredicate     = QString::fromStdString(predicate->objid());
    }

    // Retrieve the triple's object information

    ObjRef<iface::rdf_api::Node> object = pTriple->object();
    ObjRef<iface::rdf_api::URIReference> objectUriReference;
    QUERY_INTERFACE(objectUriReference, object, rdf_api::URIReference);

qDebug("---------------------------------------");
qDebug("Object - Supported interfaces:");
supportedInterfaces = object->supported_interfaces();
for (int i = 0, iMax = supportedInterfaces.size(); i < iMax; ++i)
    qDebug(" - %s", supportedInterfaces[i].c_str());

    if (objectUriReference) {
        mObjectType  = Uri;
        mObject      = QString::fromStdWString(objectUriReference->URI());
        mObjectExtra = QString();
    } else {
        ObjRef<iface::rdf_api::PlainLiteral> objectPlainLiteral;
        QUERY_INTERFACE(objectPlainLiteral, object, rdf_api::PlainLiteral);

        if (objectPlainLiteral) {
            mObjectType  = PlainLiteral;
            mObject      = QString::fromStdWString(objectPlainLiteral->lexicalForm());
            mObjectExtra = QString::fromStdWString(objectPlainLiteral->language());
        } else {
            ObjRef<iface::rdf_api::TypedLiteral> objectTypedLiteral;
            QUERY_INTERFACE(objectTypedLiteral, object, rdf_api::TypedLiteral);

            if (objectTypedLiteral) {
                mObjectType  = TypedLiteral;
                mObject      = QString::fromStdWString(objectTypedLiteral->lexicalForm());
                mObjectExtra = QString::fromStdWString(objectTypedLiteral->datatypeURI());
            } else {
                mObjectType  = ObjId;
                mObject      = QString::fromStdString(object->objid());
                mObjectExtra = QString();
            }
        }
    }

qDebug("---------------------------------------");
qDebug("Subject: %s [%d]", qPrintable(mSubject), mSubjectType);
qDebug("Predicate: %s [%d]", qPrintable(mPredicate), mPredicateType);
qDebug("Object: %s ¦ %s [%d]", qPrintable(mObject), qPrintable(mObjectExtra), mObjectType);
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriple::subjectType() const
{
    // Return the RDF triple's subject's type

    return mSubjectType;
}

//==============================================================================

QString CellmlFileRdfTriple::subject() const
{
    // Return the RDF triple's subject

    return mSubject;
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriple::predicateType() const
{
    // Return the RDF triple's predicate's type

    return mPredicateType;
}

//==============================================================================

QString CellmlFileRdfTriple::predicate() const
{
    // Return the RDF triple's predicate

    return mPredicate;
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriple::objectType() const
{
    // Return the RDF triple's object's type

    return mObjectType;
}

//==============================================================================

QString CellmlFileRdfTriple::object() const
{
    // Return the RDF triple's object

    return mObject;
}

//==============================================================================

QString CellmlFileRdfTriple::objectExtra() const
{
    // Return the RDF triple's object extra information

    return mObjectExtra;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
