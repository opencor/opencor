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
// XSL transformer
//==============================================================================

#include "corecliutils.h"
#include "xsltransformer.h"

//==============================================================================

#include <QThread>
#include <QXmlQuery>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

XslTransformerWorker::XslTransformerWorker(const QString &pInput,
                                           const QString &pXsl) :
    mInput(pInput),
    mXsl(pXsl)
{
}

//==============================================================================

void XslTransformerWorker::run()
{
    // Create our XML query object

    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler dummyMessageHandler;

    xmlQuery.setMessageHandler(&dummyMessageHandler);

    // Customise our XML query object

    xmlQuery.setFocus(mInput);
    xmlQuery.setQuery(mXsl);

    // Do the XSL transformation

    QString output;

    if (!xmlQuery.evaluateTo(&output)) {
        output = QString();
    }

    // Let people know that our XSL transformation is done

    emit done(mInput, output);
}

//==============================================================================

void XslTransformer::transform(const QString &pInput, const QString &pXsl)
{
    // Create and move our worker to a thread

    auto thread = new QThread();
    auto worker = new XslTransformerWorker(pInput, pXsl);

    worker->moveToThread(thread);

    connect(thread, &QThread::started,
            worker, &XslTransformerWorker::run);

    connect(worker, &XslTransformerWorker::done,
            this, &XslTransformer::done);
    connect(worker, &XslTransformerWorker::done,
            thread, &QThread::quit);
    connect(worker, &XslTransformerWorker::done,
            worker, &XslTransformerWorker::deleteLater);

    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);

    // Start our worker by starting the thread in which it is

    thread->start();
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
