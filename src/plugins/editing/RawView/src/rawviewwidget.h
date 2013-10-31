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
// Raw view widget
//==============================================================================

#ifndef RAWVIEWWIDGET_H
#define RAWVIEWWIDGET_H

//==============================================================================

#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

class RawViewWidget : public QScintillaSupport::QScintillaWidget
{
    Q_OBJECT

public:
    explicit RawViewWidget(const QString &pFileName, QWidget *pParent = 0);

    void fileReloaded();
    void fileRenamed(const QString &pFileName);

private:
    QString mFileName;
};

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
