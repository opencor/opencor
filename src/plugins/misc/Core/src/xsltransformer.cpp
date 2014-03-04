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
// XSL transformation
//==============================================================================

#include "cliutils.h"
#include "xsltransformer.h"

//==============================================================================

#include <QAbstractMessageHandler>
#include <QMutex>
#include <QThread>
#include <QXmlQuery>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class DummyMessageHandler : public QAbstractMessageHandler
{
protected:
    virtual void handleMessage(QtMsgType pType, const QString &pDescription,
                               const QUrl &pIdentifier,
                               const QSourceLocation &pSourceLocation)
    {
        Q_UNUSED(pType);
        Q_UNUSED(pDescription);
        Q_UNUSED(pIdentifier);
        Q_UNUSED(pSourceLocation);

        // We ignore the message...
    }
};

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
    mJobs(QList<XslTransformerJob *>())
{
    // Create our thread
    // Note: XSL transformation requires using a QXmlQuery object, which is
    //       inherently not thread-safe. So, we create a thread and move
    //       ourselves to it...

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

    mJobs << new XslTransformerJob(pInput, pXsl);

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

    QXmlQuery *xmlQuery = new QXmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler *dummyMessageHandler = new DummyMessageHandler();

    xmlQuery->setMessageHandler(dummyMessageHandler);

    // Do our XSL transformations

    QMutex pausedMutex;

    while (!mStopped) {
        // Carry out our different jobs

        while (mJobs.count() && !mStopped) {
            // Retrieve the first job in our list

            XslTransformerJob *job = mJobs.first();

            mJobs.removeFirst();

            // Customise our XML query object

            xmlQuery->setFocus(job->input());
            xmlQuery->setQuery(job->xsl());

            // Do the XSL transformation

            QString output;

            if (!xmlQuery->evaluateTo(&output))
                output = QString();

            // Let people know that an XSL transformation has been performed

            emit done(job->input(), output);

            // We are done with our job, so...

            delete job;
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

    // We are done, so clean up after ourselves
    // Note: we clean up our remaining jobs in case we were asked to stop before
    //       we had time to carry them out...

    delete xmlQuery;
    delete dummyMessageHandler;

qDebug(">>> Nb of jobs: %d", mJobs.count());

    foreach (XslTransformerJob *job, mJobs)
        delete job;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
