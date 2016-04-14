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

#include "corecliutils.h"
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

void MathmlTests::initTestCase()
{
    // We don't want to see warning messages (about QXmlQuery and threading)

    gOrigMessageHandler = qInstallMessageHandler(messageHandler);
}

//==============================================================================

void MathmlTests::tests(const QString &pCategory)
{
    // Convert some Content MathML to Presentation MathML

    QString dirName = OpenCOR::dirName("src/plugins/miscellaneous/Core/tests/data")+QDir::separator()+pCategory+QDir::separator();
    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler dummyMessageHandler;
    QString actualOutput;
    QString expectedOutput;
    QString failMessage = QString();

    xmlQuery.setMessageHandler(&dummyMessageHandler);

    foreach (const QString &fileName, QDir(dirName).entryList(QStringList() << "*.in")) {
        xmlQuery.setFocus(OpenCOR::rawFileContents(dirName+fileName));
        xmlQuery.setQuery(OpenCOR::rawFileContents(":ctop.xsl"));

        if (xmlQuery.evaluateTo(&actualOutput)) {
            actualOutput = OpenCOR::Core::formatXml(OpenCOR::Core::cleanPresentationMathml(actualOutput));
            expectedOutput = OpenCOR::rawFileContents(QString(dirName+fileName).replace(".in", ".out"));

            if (actualOutput.compare(expectedOutput)) {
                if (!failMessage.isEmpty())
                    failMessage += QString("\nFAIL!  : MathmlTests::%1Tests() ").arg(pCategory);

                failMessage += QString("Failed to convert '%1/%2'\n%3\n%4\n%5").arg(pCategory, fileName, OpenCOR::rawFileContents(dirName+fileName), actualOutput, expectedOutput);
            }
        } else {
            if (!failMessage.isEmpty())
                failMessage += QString("\nFAIL!  : MathmlTests::%1Tests() ").arg(pCategory);

            failMessage += QString("Could not convert '%1/%2'").arg(pCategory, fileName);
        }
    }

    if (!failMessage.isEmpty())
        QFAIL(qPrintable(failMessage));
}

//==============================================================================

void MathmlTests::plusTests()
{
    // Run some tests for our plus category

    tests("plus");
}

//==============================================================================

void MathmlTests::minusTests()
{
    // Run some tests for our minus category

    tests("minus");
}

//==============================================================================

void MathmlTests::timesTests()
{
    // Run some tests for our times category

    tests("times");
}

//==============================================================================

void MathmlTests::divideTests()
{
    // Run some tests for our divide category

    tests("divide");
}

//==============================================================================

void MathmlTests::powerTests()
{
    // Run some tests for our power category

    tests("power");
}

//==============================================================================

void MathmlTests::rootTests()
{
    // Run some tests for our root category

    tests("root");
}

//==============================================================================

void MathmlTests::absTests()
{
    // Run some tests for our abs category

    tests("abs");
}

//==============================================================================

void MathmlTests::expTests()
{
    // Run some tests for our exp category

    tests("exp");
}

//==============================================================================

void MathmlTests::lnTests()
{
    // Run some tests for our ln category

    tests("ln");
}

//==============================================================================

void MathmlTests::logTests()
{
    // Run some tests for our log category

    tests("log");
}

//==============================================================================

void MathmlTests::floorTests()
{
    // Run some tests for our floor category

    tests("floor");
}

//==============================================================================

void MathmlTests::ceilTests()
{
    // Run some tests for our ceil category

    tests("ceil");
}

//==============================================================================

void MathmlTests::factTests()
{
    // Run some tests for our fact category

    tests("fact");
}

//==============================================================================

void MathmlTests::remTests()
{
    // Run some tests for our rem category

    tests("rem");
}

//==============================================================================

void MathmlTests::minTests()
{
    // Run some tests for our min category

    tests("min");
}

//==============================================================================

void MathmlTests::maxTests()
{
    // Run some tests for our max category

    tests("max");
}

//==============================================================================

void MathmlTests::gcdTests()
{
    // Run some tests for our gcd category

    tests("gcd");
}

//==============================================================================

void MathmlTests::lcmTests()
{
    // Run some tests for our lcm category

    tests("lcm");
}

//==============================================================================

void MathmlTests::trigonometricTests()
{
    // Run some tests for our trigonometric category

    tests("trigonometric");
}

//==============================================================================

QTEST_APPLESS_MAIN(MathmlTests)

//==============================================================================
// End of file
//==============================================================================
