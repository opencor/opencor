/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML file RDF triple element
//==============================================================================

#include "cellmlfilerdftripleelement.h"

//==============================================================================

#include <QMap>

//==============================================================================

#include "cellmlapibegin.h"
    #include "cellml-api-cxx-support.hpp"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTripleElement::CellmlFileRdfTripleElement(iface::rdf_api::Node *pRdfNode) :
    mId(QString()),
    mUriReference(QString()),
    mLexicalForm(QString()),
    mLanguage(QString()),
    mDataTypeUri(QString())
{
    // Check which interface is supported by the node and initialise it in case
    // it supports the rdf_api::URIReference, rdf_api::PlainLiteral or
    // rdf_api::TypedLiteral interface

    ObjRef<iface::rdf_api::URIReference> uriReference = QueryInterface(pRdfNode);

    if (uriReference) {
        // The rdf_api::URIReference interface is supported, so initialise the
        // triple element using that interface

        mType = UriReference;

        mUriReference = QString::fromStdWString(uriReference->URI()).trimmed();
    } else {
        ObjRef<iface::rdf_api::PlainLiteral> plainLiteral = QueryInterface(pRdfNode);

        if (plainLiteral) {
            // The rdf_api::PlainLiteral interface is supported, so initialise
            // the triple element using that interface

            mType = PlainLiteral;

            mLexicalForm = QString::fromStdWString(plainLiteral->lexicalForm()).trimmed();
            mLanguage = QString::fromStdWString(plainLiteral->language()).trimmed();
        } else {
            ObjRef<iface::rdf_api::TypedLiteral> typedLiteral = QueryInterface(pRdfNode);

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

                QString id = QString::fromStdString(pRdfNode->objid()).trimmed();

                mType = Id;

                mId = ids.value(id);

                if (mId == QString()) {
                    // There is no id value for the current id, so generate one
                    // and keep track of it

                    mId = QString("id_%1").arg(++counter, 9, 10, QChar('0'));

                    ids.insert(id, mId);
                }
            }
        }
    }

    // Keep track of the RDF triple element's value as a string, which is based
    // on its type

    switch (mType) {
    case Id:
        mAsString = mId;

        break;
    case UriReference:
        mAsString = mUriReference;

        break;
    case PlainLiteral:
        mAsString = mLexicalForm+" ["+mLanguage+"]";

        break;
    case TypedLiteral:
        mAsString = mLexicalForm+" ["+mDataTypeUri+"]";

        break;
    }
}

//==============================================================================

CellmlFileRdfTripleElement::CellmlFileRdfTripleElement(const QString &pUriReference) :
    mType(UriReference),
    mId(QString()),
    mUriReference(pUriReference),
    mLexicalForm(QString()),
    mLanguage(QString()),
    mDataTypeUri(QString()),
    mAsString(pUriReference)
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
    // Return the RDF triple element's value as a string

    return mAsString;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
