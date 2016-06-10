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
// CellML Text view CLI tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "clitests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CliTests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/help.out"));

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::help"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::unknown"),
             help);
}

//==============================================================================

void CliTests::exportTests()
{
}

//==============================================================================

void CliTests::importTests()
{
}

//==============================================================================

QTEST_GUILESS_MAIN(CliTests)

//==============================================================================
// End of file
//==============================================================================
