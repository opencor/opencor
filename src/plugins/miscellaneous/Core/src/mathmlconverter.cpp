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
// MathML conversion
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

    connect(mXslTransformer, SIGNAL(done(const QString &, const QString &)),
            this, SLOT(xslTransformationDone(const QString &, const QString &)));
}

//==============================================================================

MathmlConverter::~MathmlConverter()
{
    // Stop our XSL transformer
    // Note: we don't need to delete it since it will be done as part of its
    //       thread being stopped...

    mXslTransformer->stop();
}

//==============================================================================

void MathmlConverter::convert(const QString &pContentMathml)
{
    // Convert the given Content MathML to Presentation MathML through an XSL
    // transformation

    static const QString CtopXsl = resource(":ctop.xsl");

    mXslTransformer->transform(pContentMathml, CtopXsl);
}

//==============================================================================

void MathmlConverter::xslTransformationDone(const QString &pInput,
                                            const QString &pOutput)
{
    // Let people know that our MathML conversion has been performed (after
    // having cleaned up its output)

    emit done(pInput, cleanPresentationMathml(pOutput));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
