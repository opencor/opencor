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
// Sample view widget
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "sampleviewwidget.h"

//==============================================================================

#include "ui_sampleviewwidget.h"

//==============================================================================

#include <QFile>

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

SampleViewWidget::SampleViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::SampleViewWidget),
    mFileName(QString())
{
    // Delete the layout that comes with ViewWidget

    delete layout();

    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

SampleViewWidget::~SampleViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SampleViewWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Update ourself too since some widgets will have been reset following the
    // retranslation (e.g. mGui->fileNameValue)

    update(mFileName);
}

//==============================================================================

QWidget * SampleViewWidget::widget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // Return the requested (self) widget

    return this;
}

//==============================================================================

void SampleViewWidget::update(const QString &pFileName)
{
    // Keep track of the given file name

    mFileName = pFileName;

    // Initialise our GUI with some information about the given file

    mGui->fileNameValue->setText(pFileName);

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    mGui->lockedValue->setText(fileManagerInstance->isLocked(pFileName)?tr("Yes"):tr("No"));

    QString sha1Value = fileManagerInstance->sha1(pFileName);

    mGui->sha1Value->setText(sha1Value.isEmpty()?"???":sha1Value);
    mGui->sizeValue->setText(Core::sizeAsString(QFile(pFileName).size()));
}

//==============================================================================

}   // namespace SampleView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
