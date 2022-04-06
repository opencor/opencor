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
    // Check whether we want our output in a file

    bool logMode = (pArgC > 1) && (strcmp(pArgV[1], "-l") == 0);

    // Retrieve the requested tests, if any

    QStringList requestedTests;

    for (int i = 1+(logMode?1:0); i < pArgC; ++i) {
        requestedTests << pArgV[i];
    }

    // The different groups of tests that are to be run
    // Note: -1 for iMax because tests ends with our separator...

    QStringList testsList = OpenCOR::fileContents(":/tests");
    QString tests = testsList.first();
    QMap<QString, QStringList> testsGroups;
    QStringList testItems = tests.split('|');
    QString testGroup;
    QString testTest;
    bool addTest;
    int nbOfTests = 0;

    for (int i = 0, iMax = testItems.count()-1; i < iMax; i += 2) {
        testGroup = testItems[i];
        testTest = testItems[i+1];

        if (pArgC == 1+(logMode?1:0)) {
            addTest = true;
        } else {
            addTest = false;

            for (const auto &requestedTest : qAsConst(requestedTests)) {
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

    QStringList buildDirList = OpenCOR::fileContents(":/build_directory");
    QString buildDir = buildDirList.first();

#ifdef Q_OS_WIN
    QDir::setCurrent(buildDir+"/plugins/OpenCOR");
#endif

    // Run the different tests

    int res = 0;
    QProcess process;
    QStringList failedTests;

    process.setProcessChannelMode(QProcess::MergedChannels);

    auto testBegin = testsGroups.constBegin();
    auto testEnd = testsGroups.constEnd();

    QFile logFile(QFileInfo(pArgV[0]).canonicalFilePath()+".log");
    QFile stdoutFile;
    QFile &file = logMode?logFile:stdoutFile;

    if (logMode) {
        file.open(QIODevice::WriteOnly);
    } else {
        file.open(stdout, QIODevice::WriteOnly);
    }

    for (auto testsGroup = testBegin; testsGroup != testEnd; ++testsGroup) {
        if (testsGroup != testBegin) {
            file.write("\n");
            file.write("\n");
            file.write("\n");
        }

        file.write(QString("********* %1 *********\n").arg(testsGroup.key()).toUtf8());
        file.write("\n");

        for (const auto &testName : testsGroup.value()) {
            // Execute the test itself

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
            process.start(buildDir+"/bin/"+testsGroup.key()+"_"+testName, QStringList());
#else
            process.start(buildDir+"/OpenCOR.app/Contents/MacOS/"+testsGroup.key()+"_"+testName, QStringList());
#endif

            process.waitForFinished(-1);

            file.write(process.readAll()+"\n");

            if (process.exitCode() != 0) {
                failedTests << testsGroup.key()+"::"+testName;
            }

            res = (res != 0)?res:process.exitCode();
        }

        file.write((QString("*").repeated(9+1+testsGroup.key().count()+1+9)+"\n").toUtf8());
    }

    // Reporting

    if (nbOfTests != 0) {
        file.write("\n");
        file.write("\n");
        file.write("\n");
    }

    file.write("********* Reporting *********\n");
    file.write("\n");

    if (failedTests.isEmpty()) {
        if (nbOfTests == 0) {
            file.write("No tests were run!\n");
        } else if (nbOfTests == 1) {
            file.write("The test passed!\n");
        } else {
            file.write("All the tests passed!\n");
        }
    } else {
        if (failedTests.count() == 1) {
            file.write("The following test failed:\n");
        } else {
            file.write("The following tests failed:\n");
        }

        for (const auto &failedTest : qAsConst(failedTests)) {
            file.write(QString(" - %s\n").arg(failedTest).toUtf8());
        }
    }

    file.write("\n");
    file.write("*****************************\n");

    file.close();

    // Return the overall outcome of the tests

    return res;
}

//==============================================================================
// End of file
//==============================================================================
