//==============================================================================
// Main source file
//==============================================================================

#include <QCoreApplication>
#include <QMap>
#include <QProcess>
#include <QString>

//==============================================================================

#include <iostream>

//==============================================================================

typedef QMap<QString, QStringList> Tests;

//==============================================================================

int main(int pArgc, char *pArgv[])
{
    // Retrieve the different arguments that were passed

    QStringList args = QStringList();

    for (int i = 1; i < pArgc; ++i)
        args << pArgv[i];

    // The different tests that are  to be run

    Tests tests;

    tests["CellMLSupport"] = QStringList() << "test";

    // Run the different tests

    QString exePath = QCoreApplication(pArgc, pArgv).applicationDirPath();
    QStringList failedTests = QStringList();
    int res = 0;

    Tests::const_iterator iter = tests.constBegin();

    while (iter != tests.constEnd()) {
        if (iter != tests.constBegin()) {
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }

        std::cout << "********* " << qPrintable(iter.key()) << " *********" << std::endl;
        std::cout << std::endl;

        foreach (const QString &test, iter.value()) {
            QString testName = QString("%1_%2").arg(iter.key(), test);

            // On Linux and Mac OS X, if we want to load plugins, we must
            // execute the test from the directory where the test is, so...

#ifndef Q_WS_WIN
            ::chdir(qPrintable(exePath));
#endif

            // Execute the test itself

            int testRes = QProcess::execute(QString("%1/%2").arg(exePath, testName), args);

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
