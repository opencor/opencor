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
// MathML converter
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

signals:
    void done(const QString &pContentMathml,
              const QString &pPresentationMathml);

private slots:
    void xslTransformationDone(const QString &pInput, const QString &pOutput);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
