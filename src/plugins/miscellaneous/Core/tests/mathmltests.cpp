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
// Core MathML tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "mathmltests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <QXmlQuery>

//==============================================================================

void DummyMessageHandler::handleMessage(QtMsgType pType,
                                        const QString &pDescription,
                                        const QUrl &pIdentifier,
                                        const QSourceLocation &pSourceLocation)
{
    Q_UNUSED(pType);
    Q_UNUSED(pDescription);
    Q_UNUSED(pIdentifier);
    Q_UNUSED(pSourceLocation);

    // We ignore the message...
}

//==============================================================================

QtMessageHandler gOrigMessageHandler;

//==============================================================================

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Handle, using the default message handler, all messages except warning
    // messages

    if (type != QtWarningMsg)
        gOrigMessageHandler(type, context, msg);
}

//==============================================================================

void MathmlTests::tests()
{
    // We don't want to see warning messages (about QXmlQuery and threading)

    gOrigMessageHandler = qInstallMessageHandler(messageHandler);

    // Convert some Content MathML to Presentation MathML

    QString dirName = OpenCOR::dirName("src/plugins/miscellaneous/Core/tests/data")+QDir::separator();
    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler dummyMessageHandler;
    QString output;

    xmlQuery.setMessageHandler(&dummyMessageHandler);

    foreach (const QString &fileName, QDir(dirName).entryList(QStringList() << "*.in")) {
        xmlQuery.setFocus(OpenCOR::rawFileContents(dirName+fileName));
        xmlQuery.setQuery(OpenCOR::rawFileContents(":ctop.xsl"));

        QVERIFY(xmlQuery.evaluateTo(&output));

        QCOMPARE(output, QString(OpenCOR::rawFileContents(QString(dirName+fileName).replace(".in", ".out"))));
    }
}

//==============================================================================

QTEST_GUILESS_MAIN(MathmlTests)

//==============================================================================
// End of file
//==============================================================================
