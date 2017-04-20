/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Core CLI tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "clitests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CliTests::cliTests()
{
    // Ask for OpenCOR's CLI help

    QCOMPARE(OpenCOR::runCli(QStringList() << "-h"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/help.out")));
    QCOMPARE(OpenCOR::runCli(QStringList() << "-x"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/help.out")));

    // Ask for the about information
    // Note: we only check the last line since the other ones are version, year
    //       and system dependent...

    QStringList output = OpenCOR::runCli(QStringList() << "-a");

    QCOMPARE(QStringList() << output[output.count()-2] << QString(),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/about.out")));

    // List the CLI plugins

#ifdef ENABLE_SAMPLE_PLUGINS
    QCOMPARE(OpenCOR::runCli(QStringList() << "-p"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/plugins_with_sample_tools.out")));
#else
    QCOMPARE(OpenCOR::runCli(QStringList() << "-p"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/plugins_without_sample_tools.out")));
#endif

    // List the status of all the plugins

#ifdef ENABLE_SAMPLE_PLUGINS
    QCOMPARE(OpenCOR::runCli(QStringList() << "-s"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/status_with_sample_tools.out")));
#else
    QCOMPARE(OpenCOR::runCli(QStringList() << "-s"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/status_without_sample_tools.out")));
#endif

    // Try an unknown plugin

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "UnknownPlugin"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_plugin.out")));

    // Try an unknown command

#ifdef ENABLE_SAMPLE_PLUGINS
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "::UnknownCommand"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_command_with_sample_tools.out")));
#else
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "::UnknownCommand"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/Core/tests/data/cli/unknown_command_without_sample_tools.out")));
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(CliTests)

//==============================================================================
// End of file
//==============================================================================
