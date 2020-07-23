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
    mGui(new Ui::MappingViewWidget),
    mListOutput(nullptr)
{
    // Delete the layout that comes with ViewWidget

    mOutputFileName = "../opencor/meshes/circulation.exnode";

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

    update(mFileName);
}

//==============================================================================

QWidget * MappingViewWidget::widget(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // Return the requested (self) widget

    return this;
}

//==============================================================================

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

    mListViewModelVariables = new QStringListModel(); //TODO defining only when charging the plugin ?
    mListViewModelOutput = new QStringListModel();

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

void MappingViewWidget::populateCellmlModel()
{
    QStringList list;

    // Make sure that we have a model before actually populating ourselves

    iface::cellml_api::Model *cellmlModel = mCellmlFile->model();

    if (cellmlModel == nullptr) {
        return;
    }

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = cellmlModel->localComponents();

    if (components->length() != 0) {

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIter = components->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = componentsIter->nextComponent();
             component != nullptr; component = componentsIter->nextComponent()) {

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length() != 0) {
                // Retrieve the model's component's variables themselves

                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIter = componentVariables->iterateVariables();

                for (ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIter->nextVariable();
                     componentVariable != nullptr; componentVariable = componentVariablesIter->nextVariable()) {

                    list.append(QString::fromStdWString(componentVariable->name()));
                }
            }
        }
    }

    mListViewModelVariables->setStringList(list);
}

void MappingViewWidget::updateOutput()
{
    if (mListOutput != nullptr) {
        delete mListOutput;
    }

    meshReader reader(mOutputFileName);

    mListOutput = new QStringList(reader.getNodesNames());
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
