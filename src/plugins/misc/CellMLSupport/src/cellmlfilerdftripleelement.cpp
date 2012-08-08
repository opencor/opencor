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
    mId(QString()),
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
                // The node doesn't support any interface, so initialise the
                // triple element using its id
                // Note: the id returned by the CellML API will look something
                //       like
                //
                //          7EQ?;?Y?A?w???A
                //
                //       This is clearly not user-friendly, so we generate and
                //       use our own id instead...

                static QMap<QString, QString> ids;
                static int counter = 0;

                QString id = QString::fromStdString(pNode->objid()).trimmed();

                mType = Id;

                mId = ids.value(id);

                if (mId == QString()) {
                    // There is no genId value for the current id, so generate
                    // one and keep track of it

                    mId = QString("id_%1").arg(++counter, 5, 10, QChar('0'));

                    ids.insert(id, mId);
                }
            }
        }
    }
}

//==============================================================================

CellmlFileRdfTripleElement::CellmlFileRdfTripleElement(const QString &pUriReference) :
    mType(UriReference),
    mId(QString()),
    mUriReference(pUriReference),
    mLexicalForm(QString()),
    mLanguage(QString()),
    mDataTypeUri(QString())
{
}

//==============================================================================

CellmlFileRdfTripleElement::Type CellmlFileRdfTripleElement::type() const
{
    // Return the RDF triple element's type

    return mType;
}

//==============================================================================

QString CellmlFileRdfTripleElement::id() const
{
    // Return the RDF triple element's id

    return mId;
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
        // Id

        return mId;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
