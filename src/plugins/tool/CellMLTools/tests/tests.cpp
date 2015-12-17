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
// CellML tools tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::initTestCase()
{
    // Get a temporary file name for our exported file

    mFileName = OpenCOR::Core::temporaryFileName();
}

//==============================================================================

void Tests::cleanupTestCase()
{
    // Delete our exported file

    QFile::remove(mFileName);
}

//==============================================================================

void Tests::cliHelpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/help.out"));

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown"),
             help);
}

//==============================================================================

void Tests::cliCellmlExportTests()
{
    // Try to export a CellML 1.0 file to CellML 1.0

    QString inFileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/noble_model_1962.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << mFileName << "cellml_1_0"),
             QStringList() << "The input file is already a CellML 1.0 file." << QString());

    // Export a CellML 1.1 file to CellML 1.0

    inFileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/experiments/periodic-stimulus.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << mFileName << "cellml_1_0"),
             QStringList() << QString());
    QCOMPARE(OpenCOR::fileContents(mFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/cellml_1_0_export.out")));

    // Try to export a non-existing CellML file to CellML 1.0

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_input_file" << mFileName << "cellml_1_0"),
             QStringList() << "The input file could not be found." << QString());

    // Try to export to a user-defined format, which file description doesn't
    // exist

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << mFileName << "non_existing_user_defined_format_file"),
             QStringList() << "The user-defined format file could not be found." << QString());

    // Try to export a local file to a user-defined format, which file
    // description exists

    QString userDefinedFormatFileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << mFileName << userDefinedFormatFileName),
             QStringList() << QString());
#ifdef Q_OS_WIN
    QCOMPARE(OpenCOR::fileContents(mFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format_export_on_windows.out")));
#else
    QCOMPARE(OpenCOR::fileContents(mFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format_export_on_non_windows.out")));
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
