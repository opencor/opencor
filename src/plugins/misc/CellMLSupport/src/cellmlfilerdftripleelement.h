//==============================================================================
// CellML file RDF triple element
//==============================================================================

#ifndef CELLMLFILERDFTRIPLEELEMENT_H
#define CELLMLFILERDFTRIPLEELEMENT_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceRDF_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTripleElement
{
public:
    enum Type
    {
        Id,
        UriReference,
        PlainLiteral,
        TypedLiteral
    };

    explicit CellmlFileRdfTripleElement(iface::rdf_api::Node *pNode);
    explicit CellmlFileRdfTripleElement(const QString &pUriReference);

    Type type() const;

    QString id() const;

    QString uriReference() const;

    QString lexicalForm() const;
    QString language() const;
    QString dataTypeUri() const;

    QString asString() const;

private:
    Type mType;

    QString mId;

    QString mUriReference;

    QString mLexicalForm;
    QString mLanguage;
    QString mDataTypeUri;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
