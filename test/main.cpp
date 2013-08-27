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
// Main source file
//==============================================================================

#include <QDir>
#include <QMap>
#include <QProcess>
#include <QString>

//==============================================================================

#include <iostream>

//==============================================================================

typedef QMap<QString, QStringList> Tests;

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Retrieve the different arguments that were passed

    QStringList args = QStringList();

    for (int i = 1; i < pArgC; ++i)
        args << pArgV[i];

    // The different tests that are to be run

    Tests tests;

    tests["Compiler"] = QStringList() << "test";

    // Run the different tests

    QString exePath = QFileInfo(pArgV[0]).canonicalPath();
    QStringList failedTests = QStringList();
    int res = 0;

    Tests::const_iterator iterBegin = tests.constBegin();
    Tests::const_iterator iterEnd = tests.constEnd();

    Tests::const_iterator iter = iterBegin;

    while (iter != iterEnd) {
        if (iter != iterBegin) {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }

        std::cout << "********* " << qPrintable(iter.key()) << " *********" << std::endl;
        std::cout << std::endl;

        foreach (const QString &test, iter.value()) {
            QString testName = QString("%1_%2").arg(iter.key(), test);

            // Go to the parent directory of the directory that contains the
            // test, so that we can load plugins without any problem

            QDir::setCurrent(exePath+"/..");

            // Execute the test itself

            int testRes = QProcess::execute(exePath+QDir::separator()+testName, args);

            if (testRes)
                failedTests << testName;

            res = res?res:testRes;

            std::cout << std::endl;
        }

        std::cout << qPrintable(QString("*").repeated(9+1+iter.key().count()+1+9)) << std::endl;

        ++iter;
    }

    // Reporting

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "********* Reporting *********" << std::endl;
    std::cout << std::endl;

    if (failedTests.isEmpty()) {
        std::cout << "All the tests passed!" << std::endl;
    } else {
        if (failedTests.count() == 1)
            std::cout << "The following test failed:" << std::endl;
        else
            std::cout << "The following tests failed:" << std::endl;

        foreach (const QString &failedTest, failedTests)
            std::cout << " - " << qPrintable(failedTest) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "*****************************" << std::endl;

    // Return the overall outcome of the tests

    return res;
}

//==============================================================================
// End of file
//==============================================================================
