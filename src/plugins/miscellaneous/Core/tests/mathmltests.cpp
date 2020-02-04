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

static QtMessageHandler gOrigMessageHandler;

//==============================================================================

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Handle, using the default message handler, all messages except warning
    // messages

    if (type != QtWarningMsg) {
        gOrigMessageHandler(type, context, msg);
    }
}

//==============================================================================

void MathmlTests::initTestCase()
{
    // We don't want to see warning messages (about QXmlQuery and threading)

    gOrigMessageHandler = qInstallMessageHandler(messageHandler);

    // Retrieve our query

    mQuery = OpenCOR::rawFileContents(":/Core/web-xslt/ctopff.xsl");
}

//==============================================================================

void MathmlTests::tests(const QString &pCategory)
{
    // Convert some Content MathML to Presentation MathML

    QString dirName = OpenCOR::dirName("src/plugins/miscellaneous/Core/tests/data")+"/"+pCategory+"/";
    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    OpenCOR::Core::DummyMessageHandler dummyMessageHandler;
    QString actualOutput;
    QString expectedOutput;
    QString failMessage;

    xmlQuery.setMessageHandler(&dummyMessageHandler);

    for (const auto &fileName : QDir(dirName).entryList({ "*.in" })) {
        QString focus = OpenCOR::rawFileContents(dirName+fileName);

        xmlQuery.setFocus(focus);
        xmlQuery.setQuery(mQuery);

        if (xmlQuery.evaluateTo(&actualOutput)) {
            actualOutput = OpenCOR::Core::formatXml(OpenCOR::Core::cleanPresentationMathml(actualOutput));
            expectedOutput = OpenCOR::rawFileContents(QString(dirName+fileName).replace(".in", ".out"));

            if (actualOutput != expectedOutput) {
                if (!failMessage.isEmpty()) {
                    failMessage += QString("\nFAIL!  : MathmlTests::%1Tests() ").arg(pCategory);
                }

                failMessage += QString("Failed to convert '%1/%2'\n%3\n%4\n%5").arg(pCategory, fileName, focus, actualOutput, expectedOutput);
            }
        } else {
            if (!failMessage.isEmpty()) {
                failMessage += QString("\nFAIL!  : MathmlTests::%1Tests() ").arg(pCategory);
            }

            failMessage += QString("Could not convert '%1/%2'").arg(pCategory, fileName);
        }
    }

    if (!failMessage.isEmpty()) {
        QFAIL(qPrintable(failMessage));
    }
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

QTEST_GUILESS_MAIN(MathmlTests)

//==============================================================================
// End of file
//==============================================================================
