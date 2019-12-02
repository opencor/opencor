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
// Core CLI tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "clitests.h"
#include "corecliutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CliTests::cliAboutTests()
{
    // Ask for the about information
    // Note: we only check the last line since the other ones are version, year
    //       and system dependent...

    QVERIFY(!OpenCOR::runCli(QStringList() << "-a", mOutput));
    QCOMPARE(QStringList() << mOutput[mOutput.count()-2] << QString(),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/about.out")));
}

//==============================================================================

void CliTests::cliCommandTests()
{
    // Try the command option with an unknown plugin

    QVERIFY(OpenCOR::runCli(QStringList() << "-c" << "Unknown", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_plugin.out")));

    // Try the command option with an unknown command

#ifdef ENABLE_SAMPLE_PLUGINS
    QVERIFY(OpenCOR::runCli(QStringList() << "-c" << "::Unknown", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_command_with_sample_tools.out")));
#else
    QVERIFY(OpenCOR::runCli(QStringList() << "-c" << "::Unknown", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_command_without_sample_tools.out")));
#endif
}

//==============================================================================

void CliTests::cliExcludeTests()
{
    // Exclude some plugins

    QVERIFY(!OpenCOR::runCli(QStringList() << "-e" << "Core" << "FileBrowserWindow" << "Unknown", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/exclude.out")));
}

//==============================================================================

void CliTests::cliHelpTests()
{
    // Ask for OpenCOR's CLI help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/help.out"));

    QVERIFY(!OpenCOR::runCli(QStringList() << "-h", mOutput));
    QCOMPARE(mOutput, help);

    // Try a known command, but with the wrong number of arguments

    QVERIFY(OpenCOR::runCli(QStringList() << "-a" << "argument", mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli(QStringList() << "-h" << "argument", mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli(QStringList() << "-p" << "argument", mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli(QStringList() << "-r" << "argument", mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli(QStringList() << "-s" << "argument", mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli(QStringList() << "-v" << "argument", mOutput));
    QCOMPARE(mOutput, help);

    // Try an unknown command, resulting in the help being shown

    QVERIFY(OpenCOR::runCli(QStringList() << "-x", mOutput));
    QCOMPARE(mOutput, help);
}

//==============================================================================

void CliTests::cliIncludeTests()
{
    // Include some plugins

    QVERIFY(!OpenCOR::runCli(QStringList() << "-i" << "Core" << "FileBrowserWindow" << "Unknown", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/include.out")));
}

//==============================================================================

void CliTests::cliPluginsTests()
{
    // List the CLI plugins

#ifdef ENABLE_SAMPLE_PLUGINS
    QVERIFY(!OpenCOR::runCli(QStringList() << "-p", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/plugins_with_sample_tools.out")));
#else
    QVERIFY(!OpenCOR::runCli(QStringList() << "-p", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/plugins_without_sample_tools.out")));
#endif
}

//==============================================================================

void CliTests::cliStatusTests()
{
    // List the status of all the plugins

#ifdef ENABLE_SAMPLE_PLUGINS
    QVERIFY(!OpenCOR::runCli(QStringList() << "-s", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/status_with_sample_tools.out")));
#else
    QVERIFY(!OpenCOR::runCli(QStringList() << "-s", mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/status_without_sample_tools.out")));
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(CliTests)

//==============================================================================
// End of file
//==============================================================================
