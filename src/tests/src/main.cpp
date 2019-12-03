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
    // Retrieve the requested tests, if any

    QStringList requestedTests;

    for (int i = 1; i < pArgC; ++i) {
        requestedTests << pArgV[i];
    }

    // The different groups of tests that are to be run
    // Note: -1 for iMax because tests ends with our separator...

    QString tests = OpenCOR::fileContents(":/tests").first();
    QMap<QString, QStringList> testsGroups;
    QStringList testItems = tests.split('|');
    QString testGroup;
    QString testTest;
    bool addTest;
    int nbOfTests = 0;

    for (int i = 0, iMax = testItems.count()-1; i < iMax; i += 2) {
        testGroup = testItems[i];
        testTest = testItems[i+1];

        if (pArgC == 1) {
            addTest = true;
        } else {
            addTest = false;

            for (const auto &requestedTest : requestedTests) {
                QStringList requestedTestItems = requestedTest.split("::");
                QString requestedTestGroup = requestedTestItems[0];
                QString requestedTestTest = (requestedTestItems.count() > 1)?requestedTestItems[1].toLower():QString();

                if (   (testGroup == requestedTestGroup)
                    && (   requestedTestTest.isEmpty()
                        || (testTest == requestedTestTest))) {
                    addTest = true;

                    break;
                }
            }
        }

        if (addTest) {
            testsGroups.insert(testGroup, QStringList(testsGroups.value(testGroup)) << testTest);

            ++nbOfTests;
        }
    }

    // On Windows, go to the directory that contains our plugins, so that we can
    // load them without any problem

    QString buildDir = OpenCOR::fileContents(":/build_directory").first();

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

        for (const auto &testName : testsGroup.value()) {
            // Execute the test itself

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
            int testRes = QProcess::execute(buildDir+"/bin/"+testsGroup.key()+"_"+testName, QStringList());
#else
            int testRes = QProcess::execute(buildDir+"/OpenCOR.app/Contents/MacOS/"+testsGroup.key()+"_"+testName, QStringList());
#endif

            if (testRes != 0) {
                failedTests << testsGroup.key()+"::"+testName;
            }

            res = (res != 0)?res:testRes;

            std::cout << std::endl;
        }

        std::cout << QString("*").repeated(9+1+testsGroup.key().count()+1+9).toStdString() << std::endl;
    }

    // Reporting

    if (nbOfTests != 0) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    std::cout << "********* Reporting *********" << std::endl;
    std::cout << std::endl;

    if (failedTests.isEmpty()) {
        if (nbOfTests == 0) {
            std::cout << "No tests were run!" << std::endl;
        } else if (nbOfTests == 1) {
            std::cout << "The test passed!" << std::endl;
        } else {
            std::cout << "All the tests passed!" << std::endl;
        }
    } else {
        if (failedTests.count() == 1) {
            std::cout << "The following test failed:" << std::endl;
        } else {
            std::cout << "The following tests failed:" << std::endl;
        }

        for (const auto &failedTest : failedTests) {
            std::cout << " - " << failedTest.toStdString() << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "*****************************" << std::endl;

    // Return the overall outcome of the tests

    return res;
}

//==============================================================================
// End of file
//==============================================================================
