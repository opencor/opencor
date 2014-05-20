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

#include "filemanager.h"
#include "sampleviewwidget.h"

//==============================================================================

#include "ui_sampleviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

SampleViewWidget::SampleViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::SampleViewWidget),
    mFileName(QString())
{
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

    // Reinitialise ourself too since some widgets will have been reset
    // following the retranslation (e.g. mGui->fileNameValue)

    initialize(mFileName);
}

//==============================================================================

void SampleViewWidget::initialize(const QString &pFileName)
{
    // Keep track of the given file name

    mFileName = pFileName;

    // Initialise our GUI with some information about the given file

    mGui->fileNameValue->setText(pFileName);

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    mGui->lockedValue->setText(fileManagerInstance->isLocked(pFileName)?tr("Yes"):tr("No"));

    QString sha1Value = fileManagerInstance->sha1(pFileName);

    mGui->sha1Value->setText(sha1Value.isEmpty()?"???":sha1Value);
}

//==============================================================================

void SampleViewWidget::finalize(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewWidget::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewWidget::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace SampleView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
