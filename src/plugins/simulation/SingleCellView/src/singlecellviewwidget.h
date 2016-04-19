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
// Single cell view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "combinearchive.h"
#include "corecliutils.h"
#include "sedmlfile.h"
#include "viewwidget.h"

//==============================================================================

namespace libsedml {
    class SedAlgorithm;
}   // namespace libsedml

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewPlugin;
class SingleCellViewSimulation;
class SingleCellViewSimulationWidget;

//==============================================================================

class SingleCellViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    enum FileType {
        CellmlFile,
        SedmlFile,
        CombineArchive
    };

    explicit SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                  QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool isIndirectRemoteFile(const QString &pFileName);

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    void fileOpened(const QString &pFileName);
    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    QStringList fileNames() const;

    SingleCellViewSimulation * simulation(const QString &pFileName) const;
    CellMLSupport::CellmlFileRuntime * runtime(const QString &pFileName) const;

    qulonglong simulationResultsSize(const QString &pFileName) const;

    void checkSimulationResults(const QString &pFileName,
                                const bool &pForceUpdateSimulationResults = false);

    void retrieveFileDetails(const QString &pFileName,
                             OpenCOR::CellMLSupport::CellmlFile *&pCellmlFile,
                             OpenCOR::SEDMLSupport::SedmlFile *&pSedmlFile,
                             OpenCOR::COMBINESupport::CombineArchive *&pCombineArchive,
                             FileType &pFileType,
                             SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                             COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues,
                             bool *pIsDirectOrIndirectRemoteFile = 0);

private:
    SingleCellViewPlugin *mPlugin;

    QString mSettingsGroup;

    QIntList mSimulationWidgetSizes;
    QIntList mContentsWidgetSizes;

    QBoolList mCollapsibleWidgetCollapsed;

    QIntList mSimulationWidgetColumnWidths;
    QIntList mSolversWidgetColumnWidths;
    QIntList mGraphsWidgetColumnWidths;
    QIntList mParametersWidgetColumnWidths;

    SingleCellViewSimulationWidget *mSimulationWidget;
    QMap<QString, SingleCellViewSimulationWidget *> mSimulationWidgets;

    QStringList mFileNames;

    QMap<QString, qulonglong> mSimulationResultsSizes;
    QStringList mSimulationCheckResults;

    QMap<QString, QString> mLocallyManagedCellmlFiles;
    QMap<QString, QString> mLocallyManagedSedmlFiles;

    void updateContentsInformationGui(SingleCellViewSimulationWidget *pSimulationWidget);

    bool sedmlAlgorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                 SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const;
    bool sedmlFileSupported(SEDMLSupport::SedmlFile *pSedmlFile,
                            SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const;

    bool combineArchiveSupported(COMBINESupport::CombineArchive *pCombineArchive,
                                 COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const;

    void retrieveCellmlFile(const QString &pFileName,
                            OpenCOR::CellMLSupport::CellmlFile *&pCellmlFile,
                            SEDMLSupport::SedmlFile *pSedmlFile,
                            const FileType &pFileType,
                            SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                            bool *pIsDirectOrIndirectRemoteFile);
    void retrieveSedmlFile(SEDMLSupport::SedmlFile *&pSedmlFile,
                           COMBINESupport::CombineArchive *pCombineArchive,
                           COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues);

private Q_SLOTS:
    void simulationWidgetSplitterMoved(const QIntList &pSizes);
    void contentsWidgetSplitterMoved(const QIntList &pSizes);

    void collapsibleWidgetCollapsed(const int &pIndex, const bool &pCollapsed);

    void simulationWidgetHeaderSectionResized(const int &pIndex,
                                              const int &pOldSize,
                                              const int &pNewSize);
    void solversWidgetHeaderSectionResized(const int &pIndex,
                                           const int &pOldSize,
                                           const int &pNewSize);
    void graphsWidgetHeaderSectionResized(const int &pIndex,
                                          const int &pOldSize,
                                          const int &pNewSize);
    void parametersWidgetHeaderSectionResized(const int &pIndex,
                                              const int &pOldSize,
                                              const int &pNewSize);

    void callCheckSimulationResults();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
