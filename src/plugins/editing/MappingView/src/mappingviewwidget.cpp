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
// Mapping view widget
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "mappingviewwidget.h"
#include "meshreader.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include "ui_mappingviewwidget.h"

//==============================================================================

#include <QFile>
#include <QtGui>

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewWidget::MappingViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::MappingViewWidget)
{
    // Delete the layout that comes with ViewWidget

    delete layout();

    // Set up the GUI

    mGui->setupUi(this);

}

//==============================================================================

MappingViewWidget::~MappingViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void MappingViewWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Update ourself too since some widgets will have been reset following the
    // retranslation (e.g. mGui->fileNameValue)

    for(auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

QWidget * MappingViewWidget::widget(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // Return the requested (self) widget

    return this;
}

//==============================================================================

void MappingViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new MappingViewEditingWidget(pFileName, this);

        mEditingWidgets.insert(pFileName, mEditingWidget);
    }

    //TODO to throw away
        mGui->fileNameValue->setText(pFileName);

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();

        mGui->lockedValue->setText(fileManagerInstance->isLocked(pFileName)?tr("Yes"):tr("No"));

        QString sha1Value = fileManagerInstance->sha1(pFileName);

        mGui->sha1Value->setText(sha1Value.isEmpty()?"???":sha1Value);
        mGui->sizeValue->setText(Core::sizeAsString(quint64(QFile(pFileName).size())));

    mGui->variablesList->setModel(mEditingWidget->listViewModelVariables()); //TODO set only when charging the plugin ?
    mGui->outputList->setModel(mEditingWidget->listViewModelOutput());
}

//==============================================================================

void MappingViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        // There is an editing widget for the given file name, so delete it and
        // remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget) {
            mEditingWidget = nullptr;
        }
    }
}

//==============================================================================

void MappingViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        editingWidget->filePermissionsChanged();
    }
}

//==============================================================================

void MappingViewWidget::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so consider it reloaded, but only if it
    // has a corresponding widget that is invisible

    QWidget *crtWidget = widget(pFileName);

    if ((crtWidget != nullptr) && !crtWidget->isVisible()) {
        fileReloaded(pFileName);
    }
}

//==============================================================================

void MappingViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void MappingViewWidget::fileRenamed(const QString &pOldFileName, const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================


bool MappingViewWidget::saveFile(const QString &pOldFileName, const QString &pNewFileName)
{
    // Save (update) the CellML file to the given file

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    return (editingWidget != nullptr)?editingWidget->cellmlFile()->update(pNewFileName):false;
}

//==============================================================================
/*
void MappingViewWidget::update(const QString &pFileName)
{
    // Keep track of the given file name

    mFileName = pFileName;

    // Initialise our GUI with some information about the given file

    mGui->fileNameValue->setText(pFileName);

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    mGui->lockedValue->setText(fileManagerInstance->isLocked(pFileName)?tr("Yes"):tr("No"));

    QString sha1Value = fileManagerInstance->sha1(pFileName);

    mGui->sha1Value->setText(sha1Value.isEmpty()?"???":sha1Value);
    mGui->sizeValue->setText(Core::sizeAsString(quint64(QFile(pFileName).size())));


    mGui->variablesList->setModel(mListViewModelVariables); //TODO set only when charging the plugin ?
    mGui->outputList->setModel(mListViewModelOutput);
    //TODO
    //mGui->tableView->setModel()


    //Retrieve The output variables

    updateOutput();
    mListViewModelOutput->setStringList(*mListOutput);

    // Retrieve the requested CellML file

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    populateCellmlModel();
}
*/

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
