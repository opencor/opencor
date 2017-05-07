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
// XSL transformer
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QList>
#include <QObject>
#include <QString>
#include <QWaitCondition>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class DummyMessageHandler;

//==============================================================================

class XslTransformerJob
{
public:
    explicit XslTransformerJob(const QString &pInput, const QString &pXsl);

    QString input() const;
    QString xsl() const;

private:
    QString mInput;
    QString mXsl;
};

//==============================================================================

class CORE_EXPORT XslTransformer : public QObject
{
    Q_OBJECT

public:
    explicit XslTransformer();

    void transform(const QString &pInput, const QString &pXsl);

    void stop();

private:
    QThread *mThread;

    bool mPaused;
    bool mStopped;

    QWaitCondition mPausedCondition;

    QList<XslTransformerJob> mJobs;

signals:
    void done(const QString &pInput, const QString &pOutput);

private slots:
    void started();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
