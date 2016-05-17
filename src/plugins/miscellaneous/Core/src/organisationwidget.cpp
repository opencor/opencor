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
// Organisation widget
//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

OrganisationWidget::OrganisationWidget(QWidget *pParent) :
    WindowWidget(pParent)
{
}

//==============================================================================

void OrganisationWidget::openFile(const QString &pFileName) const
{
    // Ask OpenCOR to open the file

    QDesktopServices::openUrl("opencor://openFile/"+pFileName);
}

//==============================================================================

void OrganisationWidget::openFiles(const QStringList &pFileNames) const
{
    // Ask OpenCOR to open the files

    QDesktopServices::openUrl("opencor://openFiles/"+pFileNames.join("|"));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
