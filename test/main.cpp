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

    int res = 0;

    QString exePath = QFileInfo(QDir::currentPath()+QDir::separator()+QString(pArgv[0])).absolutePath();
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
            res = res?res:QProcess::execute(QString("%1/%2_%3").arg(exePath, iter.key(), test), args);

            std::cout << std::endl;
        }

        std::cout << qPrintable(QString("*").repeated(9+1+iter.key().count()+1+9)) << std::endl;

        ++iter;
    }

    // Return the overall outcome of the tests

    return res;
}

//==============================================================================
// End of file
//==============================================================================
