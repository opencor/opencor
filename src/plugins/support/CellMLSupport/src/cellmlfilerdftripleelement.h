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

#pragma once

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellmlapibegin.h"
    #include "IfaceRDF_APISPEC.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRdfTripleElement
{
public:
    enum Type {
        Id,
        UriReference,
        PlainLiteral,
        TypedLiteral
    };

    explicit CellmlFileRdfTripleElement(iface::rdf_api::Node *pRdfNode);
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

    QString mAsString;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
