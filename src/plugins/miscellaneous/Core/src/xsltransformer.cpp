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

#include "corecliutils.h"
#include "xsltransformer.h"

//==============================================================================

#include <QMutex>
#include <QThread>
#include <QXmlQuery>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

XslTransformerJob::XslTransformerJob(const QString &pInput,
                                     const QString &pXsl) :
    mInput(pInput),
    mXsl(pXsl)
{
}

//==============================================================================

QString XslTransformerJob::input() const
{
    // Return our input

    return mInput;
}

//==============================================================================

QString XslTransformerJob::xsl() const
{
    // Return our XSL

    return mXsl;
}

//==============================================================================

XslTransformer::XslTransformer() :
    mPaused(false),
    mStopped(false),
    mJobs(QList<XslTransformerJob>())
{
    // Create our thread

    mThread = new QThread();

    // Move ourselves to our thread

    moveToThread(mThread);

    // Create a few connections

    connect(mThread, SIGNAL(started()),
            this, SLOT(started()));

    connect(mThread, SIGNAL(finished()),
            mThread, SLOT(deleteLater()));
    connect(mThread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
}

//==============================================================================

void XslTransformer::transform(const QString &pInput, const QString &pXsl)
{
    // Add a new job to our list

    mJobs << XslTransformerJob(pInput, pXsl);

    // Start/resume our thread, if needed

    if (!mThread->isRunning()) {
        mThread->start();
    } else {
        // Resume our thread, if needed

        if (mPaused)
            mPausedCondition.wakeOne();
    }
}

//==============================================================================

void XslTransformer::stop()
{
    // Shutdown our thread, if needed

    if (mThread->isRunning()) {
        // Ask our thread to stop

        mStopped = true;

        // Resume our thread, if needed

        if (mPaused)
            mPausedCondition.wakeOne();

        // Ask our thread to quit and wait for it to do so

        mThread->quit();
        mThread->wait();
    }
}

//==============================================================================

void XslTransformer::started()
{
    // Create our XML query object

    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler dummyMessageHandler;

    xmlQuery.setMessageHandler(&dummyMessageHandler);

    // Do our XSL transformations

    QMutex pausedMutex;

    while (!mStopped) {
        // Carry out our different jobs

        while (mJobs.count() && !mStopped) {
            // Retrieve the first job in our list

            XslTransformerJob job = mJobs.first();

            mJobs.removeFirst();

            // Customise our XML query object

            xmlQuery.setFocus(job.input());
            xmlQuery.setQuery(job.xsl());

            // Do the XSL transformation

            QString output;

            if (!xmlQuery.evaluateTo(&output))
                output = QString();

            // Let people know that an XSL transformation has been performed

            emit done(job.input(), output);
        }

        // Pause ourselves, unless we have been asked to stop

        if (!mStopped) {
            mPaused = true;

            pausedMutex.lock();
                mPausedCondition.wait(&pausedMutex);
            pausedMutex.unlock();

            mPaused = false;
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
