/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// MathML converter
//==============================================================================

#include "corecliutils.h"
#include "mathmlconverter.h"
#include "xsltransformer.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

MathmlConverter::MathmlConverter()
{
    // Create our XSL transformer and create a connection to retrieve the result
    // of its XSL transformations

    mXslTransformer = new XslTransformer();

    connect(mXslTransformer, &XslTransformer::done,
            this, &MathmlConverter::xslTransformationDone);
}

//==============================================================================

MathmlConverter::~MathmlConverter()
{
    // Delete our XSL transformer

    delete mXslTransformer;
}

//==============================================================================

void MathmlConverter::convert(const QString &pContentMathml)
{
    // Convert the given Content MathML to Presentation MathML through an XSL
    // transformation

    static const QString CtopXsl = resource(":/Core/web-xslt/ctopff.xsl");

    mXslTransformer->transform(pContentMathml, CtopXsl);
}

//==============================================================================

void MathmlConverter::xslTransformationDone(const QString &pInput,
                                            const QString &pOutput)
{
    // Let people know that our MathML conversion is done (after having cleaned
    // up its output)

    emit done(pInput, cleanPresentationMathml(pOutput));
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
