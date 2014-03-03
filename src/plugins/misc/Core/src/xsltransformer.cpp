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

XslTransformer::XslTransformer(const QString &pInput, const QString &pXsl) :
    QObject(),
    mInput(pInput),
    mXsl(pXsl)
{
    // Create our thread
    // Note: XSL transformation requires using a QXmlQuery object. Howwever,
    //       QXmlQuery is not thread-safe, so we create a thread and move
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

void XslTransformer::doTransformation()
{
    // Do the transformation by starting our thread

    mThread->start();
}

//==============================================================================

void XslTransformer::started()
{
    // Create and customise our XML query object

    QXmlQuery *xmlQuery = new QXmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler *dummyMessageHandler = new DummyMessageHandler();

    xmlQuery->setMessageHandler(dummyMessageHandler);

    xmlQuery->setFocus(mInput);
    xmlQuery->setQuery(mXsl);

    // Do the XSL tranformation

    QString output;

    xmlQuery->evaluateTo(&output);

    // We are done

    delete xmlQuery;
    delete dummyMessageHandler;

    emit done(mInput, output);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
