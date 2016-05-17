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

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QDomElement>
#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class XslTransformer;

//==============================================================================

class CORE_EXPORT MathmlConverter : public QObject
{
    Q_OBJECT

public:
    explicit MathmlConverter();
    ~MathmlConverter();

    void convert(const QString &pContentMathml);

private:
    XslTransformer *mXslTransformer;

Q_SIGNALS:
    void done(const QString &pContentMathml,
              const QString &pPresentationMathml);

private Q_SLOTS:
    void xslTransformationDone(const QString &pInput, const QString &pOutput);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
