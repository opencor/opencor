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
// Single cell view simulation worker
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>
#include <QWaitCondition>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationWorker(SingleCellViewSimulation *pSimulation,
                                            SingleCellViewSimulationWorker *&pSelf);

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    bool run();
    bool pause();
    bool resume();
    bool stop();

    bool reset();

private:
    QThread *mThread;

    SingleCellViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    double mCurrentPoint;

    bool mPaused;
    bool mStopped;

    bool mReset;

    QWaitCondition mPausedCondition;

    bool mError;

    SingleCellViewSimulationWorker *&mSelf;

Q_SIGNALS:
    void running(const bool &pIsResuming);
    void paused();

    void finished(const qint64 &pElapsedTime);

    void error(const QString &pMessage);

private Q_SLOTS:
    void started();

    void emitError(const QString &pMessage);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
