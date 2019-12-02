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
// Simulation worker
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
} // namespace CellMLSupport

//==============================================================================

namespace SimulationSupport {

//==============================================================================

class Simulation;

//==============================================================================

class SimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SimulationWorker(Simulation *pSimulation, QThread *pThread,
                              SimulationWorker *&pSelf);

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    void pause();
    void resume();
    void stop();

    void reset();

private:
    Simulation *mSimulation;

    QThread *mThread;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    double mCurrentPoint = 0.0;

    bool mPaused = false;
    bool mStopped = false;

    bool mReset = false;

    QWaitCondition mPausedCondition;

    bool mError = false;

    SimulationWorker *&mSelf;

signals:
    void running(bool pIsResuming);
    void paused();

    void done(qint64 pElapsedTime);

    void error(const QString &pMessage);

public slots:
    void run();

private slots:
    void emitError(const QString &pMessage);
};

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
