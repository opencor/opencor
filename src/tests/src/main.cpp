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

#include "testsutils.h"

//==============================================================================

#include <QDir>
#include <QMap>
#include <QProcess>
#include <QString>

//==============================================================================

#include <iostream>

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Retrieve the different arguments that were passed

    QStringList args = QStringList();

    for (int i = 1; i < pArgC; ++i)
        args << pArgV[i];

    // The different groups of tests that are to be run

    QString tests = OpenCOR::fileContents(":tests").first();
    QMap<QString, QStringList> testsGroups;
    QStringList testItems = tests.split("|");
    QString testGroup;

    for (int i = 0, iMax = testItems.count()-1; i < iMax; i += 2) {
        // Note: -1 because tests ends with our separator...

        testGroup = testItems[i];

        testsGroups.insert(testGroup, QStringList(testsGroups.value(testGroup)) << testItems[i+1]);
    }

    // Go to the directory that contains our plugins, so that we can load them
    // without any problem

    QString buildDir = OpenCOR::fileContents(":build_directory").first();

#ifdef Q_OS_WIN
    QDir::setCurrent(buildDir+"/plugins/OpenCOR");
#endif

    // Run the different tests

    QStringList failedTests = QStringList();
    int res = 0;

    auto testBegin = testsGroups.constBegin();
    auto testEnd = testsGroups.constEnd();

    for (auto testsGroup = testBegin; testsGroup != testEnd; ++testsGroup) {
        if (testsGroup != testBegin) {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }

        std::cout << "********* " << testsGroup.key().toStdString() << " *********" << std::endl;
        std::cout << std::endl;

        foreach (const QString &testName, testsGroup.value()) {
            // Execute the test itself

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
            int testRes = QProcess::execute(buildDir+"/bin/"+testsGroup.key()+"_"+testName, args);
#elif defined(Q_OS_MAC)
            int testRes = QProcess::execute(buildDir+"/OpenCOR.app/Contents/MacOS/"+testsGroup.key()+"_"+testName, args);
#else
    #error Unsupported platform
#endif

            if (testRes)
                failedTests << testsGroup.key()+"::"+testName;

            res = res?res:testRes;

            std::cout << std::endl;
        }

        std::cout << QString("*").repeated(9+1+testsGroup.key().count()+1+9).toStdString() << std::endl;
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
            std::cout << " - " << failedTest.toStdString() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "*****************************" << std::endl;

    // Return the overall outcome of the tests

    return res;
}

//==============================================================================
// End of file
//==============================================================================
