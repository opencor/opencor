//==============================================================================
// CellML file RDF triple element
//==============================================================================

#include "cellmlfilerdftripleelement.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTripleElement::CellmlFileRdfTripleElement(iface::rdf_api::Node *pNode) :
    mUriReference(QString()),
    mLexicalForm(QString()),
    mLanguage(QString()),
    mDataTypeUri(QString())
{
    // Check which interface is supported by the node and initialise it in case
    // it supports the rdf_api::URIReference, rdf_api::PlainLiteral or
    // rdf_api::TypedLiteral interface

    ObjRef<iface::rdf_api::URIReference> uriReference;
    QUERY_INTERFACE(uriReference, pNode, rdf_api::URIReference);

    if (uriReference) {
        // The rdf_api::URIReference interface is supported, so initialise the
        // triple element using that interface

        mType = UriReference;

        mUriReference = QString::fromStdWString(uriReference->URI()).trimmed();
    } else {
        ObjRef<iface::rdf_api::PlainLiteral> plainLiteral;
        QUERY_INTERFACE(plainLiteral, pNode, rdf_api::PlainLiteral);

        if (plainLiteral) {
            // The rdf_api::PlainLiteral interface is supported, so initialise
            // the triple element using that interface

            mType = PlainLiteral;

            mLexicalForm = QString::fromStdWString(plainLiteral->lexicalForm()).trimmed();
            mLanguage    = QString::fromStdWString(plainLiteral->language()).trimmed();
        } else {
            ObjRef<iface::rdf_api::TypedLiteral> typedLiteral;
            QUERY_INTERFACE(typedLiteral, pNode, rdf_api::TypedLiteral);

            if (typedLiteral) {
                // The rdf_api::TypedLiteral interface is supported, so
                // initialise the triple element using that interface

                mType = TypedLiteral;

                mLexicalForm = QString::fromStdWString(typedLiteral->lexicalForm()).trimmed();
                mDataTypeUri = QString::fromStdWString(typedLiteral->datatypeURI()).trimmed();
            } else {
                // The node doesn't support any interface, so initialise it
                // using only its object id
                // Note: this object id returned by the CellML API will look
                //       something like
                //
                //          7EQ?;?Y?A?w???A
                //
                //       which is not really user-friendly, so we generate our
                //       own object id...

                static QMap<QString, QString> objIds;
                static int objIdCounter = 0;

                QString objectId = QString::fromStdString(pNode->objid()).trimmed();

                mType = Object;

                mObjectId = objIds.value(objectId);

                if (mObjectId == QString()) {
                    // There is no objId value for the current object id, so
                    // generate one and keep track of it

                    mObjectId.sprintf("objid:%05d", ++objIdCounter);

                    objIds.insert(objectId, mObjectId);
                }
            }
        }
    }
}

//==============================================================================

CellmlFileRdfTripleElement::Type CellmlFileRdfTripleElement::type() const
{
    // Return the RDF triple element's type

    return mType;
}

//==============================================================================

QString CellmlFileRdfTripleElement::objectId() const
{
    // Return the RDF triple element's object id

    return mObjectId;
}

//==============================================================================

QString CellmlFileRdfTripleElement::uriReference() const
{
    // Return the RDF triple element's URI reference

    return mUriReference;
}

//==============================================================================

QString CellmlFileRdfTripleElement::lexicalForm() const
{
    // Return the RDF triple element's lexical form

    return mLexicalForm;
}

//==============================================================================

QString CellmlFileRdfTripleElement::language() const
{
    // Return the RDF triple element's language

    return mLanguage;
}

//==============================================================================

QString CellmlFileRdfTripleElement::dataTypeUri() const
{
    // Return the RDF triple element's data type URI

    return mDataTypeUri;
}

//==============================================================================

QString CellmlFileRdfTripleElement::asString() const
{
    // Return the RDF triple element's value as a string, based on its type

    switch (mType) {
    case UriReference:
        return mUriReference;
    case PlainLiteral:
        return mLexicalForm+" ["+mLanguage+"]";
    case TypedLiteral:
        return mLexicalForm+" ["+mDataTypeUri+"]";
    default:
        // None of the above, so an object

        return mObjectId;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
