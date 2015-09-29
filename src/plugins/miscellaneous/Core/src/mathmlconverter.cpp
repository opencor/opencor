/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

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

    static const QString CtopXsl = resourceAsByteArray(":ctop.xsl");

    mXslTransformer->transform(pContentMathml, CtopXsl);
}

//==============================================================================

void MathmlConverter::cleanPresentationMathmlElement(QDomElement &pDomElement) const
{
    // Merge successive child mrow elements, as long as their parent is not an
    // element that requires a specific number of arguments (which could become
    // wrong if we were to merge two successive mrow elements)
    // Note: see http://www.w3.org/TR/MathML2/chapter3.html#id.3.1.3.2 for the
    //       list of the elements to check...

    if (   pDomElement.nodeName().compare("mfrac")
        && pDomElement.nodeName().compare("mroot")
        && pDomElement.nodeName().compare("msub")
        && pDomElement.nodeName().compare("msup")
        && pDomElement.nodeName().compare("msubsup")
        && pDomElement.nodeName().compare("munder")
        && pDomElement.nodeName().compare("mover")
        && pDomElement.nodeName().compare("munderover")
        && pDomElement.nodeName().compare("munderover")
        && pDomElement.nodeName().compare("munderover")
        && pDomElement.nodeName().compare("munderover")
        && pDomElement.nodeName().compare("munderover")
        && pDomElement.nodeName().compare("munderover")) {
        for (QDomElement childElement = pDomElement.firstChildElement();
             !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
            QDomElement nextChildElement = childElement.nextSiblingElement();

            if (   !nextChildElement.isNull()
                && !childElement.nodeName().compare("mrow")
                && !childElement.nodeName().compare(nextChildElement.nodeName())) {
                // The current and next child elements are both mrow's, so merge
                // them together

                for (QDomElement nextChildChildElement = nextChildElement.firstChildElement();
                     !nextChildChildElement.isNull(); nextChildChildElement = nextChildElement.firstChildElement()) {
                    childElement.appendChild(nextChildChildElement);
                }

                pDomElement.removeChild(nextChildElement);
            }
        }
    }

    // Recursively clean ourselves

    for (QDomElement childElement = pDomElement.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        cleanPresentationMathmlElement(childElement);
    }
}

//==============================================================================

QString MathmlConverter::cleanPresentationMathml(const QString &pPresentationMathml) const
{
    // Clean the given Presentation MathML by merging successive mrow elements
    // together
    // Note: see https://github.com/opencor/opencor/issues/763...

    QDomDocument domDocument;

    domDocument.setContent(pPresentationMathml);

    QDomElement domElement = domDocument.documentElement();

    cleanPresentationMathmlElement(domElement);

    return qDomDocumentToString(domDocument);
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
