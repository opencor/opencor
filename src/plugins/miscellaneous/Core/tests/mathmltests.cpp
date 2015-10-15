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

void MathmlTests::initTestCase()
{
    // Keep track of our original directory and go to our tests directory

    mOrigPath = QDir::currentPath();

    QDir::setCurrent(OpenCOR::dirName("src/plugins/miscellaneous/Core/tests/data"));
}

//==============================================================================

void MathmlTests::cleanupTestCase()
{
    // Go back to our original directory

    QDir::setCurrent(mOrigPath);
}

//==============================================================================

void MathmlTests::tests()
{
    // Create our MathML converter and create a connection to retrieve the
    // result of its MathML conversions

    connect(&mMathmlConverter, SIGNAL(done(const QString &, const QString &)),
            this, SLOT(mathmlConversionDone(const QString &, const QString &)));

    // Convert some Content MathML to Presentation MathML

    foreach (const QString &fileName, QDir().entryList(QStringList() << "*.in")) {
        mFileNames << fileName;

        mMathmlConverter.convert(OpenCOR::rawFileContents(fileName));
    }
}

//==============================================================================

void MathmlTests::mathmlConversionDone(const QString &pContentMathml,
                                       const QString &pPresentationMathml)
{
qDebug("=========");
qDebug("%s", qPrintable(mFileNames.first()));
mFileNames.removeFirst();
qDebug("---------");
qDebug("%s", qPrintable(pContentMathml));
qDebug("---------");
qDebug("%s", qPrintable(pPresentationMathml));
}

//==============================================================================

QTEST_GUILESS_MAIN(MathmlTests)

//==============================================================================
// End of file
//==============================================================================
