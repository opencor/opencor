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
#include "mappingvieweditingwidget.h"
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

MappingViewEditingWidget::MappingViewEditingWidget(const QString &pFileName,
                                                   QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Delete the layout that comes with ViewWidget

    mOutputFileName = "../opencor/meshes/circulation.exnode";
    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mListViewModelVariables = new QStringListModel(); //TODO defining only when charging the plugin ?
    mListViewModelOutput = new QStringListModel();

    populateCellmlModel();
    populateOutput();
}

//==============================================================================

void MappingViewEditingWidget::retranslateUi()
{
    //TODO ?
}

//==============================================================================

CellMLSupport::CellmlFile * MappingViewEditingWidget::cellmlFile() const
{
    // Return our CellML file

    return mCellmlFile;
}

//==============================================================================

QStringListModel* MappingViewEditingWidget::listViewModelVariables()
{
    return mListViewModelVariables;
}

//==============================================================================

QStringListModel* MappingViewEditingWidget::listViewModelOutput()
{
    return mListViewModelOutput;
}

//==============================================================================

void MappingViewEditingWidget::filePermissionsChanged()
{
}

//==============================================================================

void MappingViewEditingWidget::populateCellmlModel()
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

void MappingViewEditingWidget::populateOutput()
{   
    meshReader reader(mOutputFileName);

    mListViewModelOutput->setStringList(reader.getNodesNames());
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
