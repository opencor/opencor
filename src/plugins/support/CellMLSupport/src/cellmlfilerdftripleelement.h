/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
