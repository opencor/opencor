/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

void Tests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/help.out"));

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools"),
             help);
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help"),
             help);

    // Try a known command, but with the wrong number of arguments

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "argument"),
             help);
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << "argument" << "argument"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown"),
             help);
}

//==============================================================================

void Tests::exportToUserDefinedFormatTests()
{
    // Try to export to a user-defined format, which file description doesn't
    // exist

    QString fileName = OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "non_existing_user_defined_format_file"),
             QStringList() << "The user-defined format file could not be found." << QString());

    // Try to export a local file to a user-defined (MATLAB) format, which file
    // description exists

    QString matlabFormatFileName = OpenCOR::fileName("formats/MATLAB.xml");

#ifdef Q_OS_WIN
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << matlabFormatFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/matlab_format_export_on_windows.out")));
#else
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << matlabFormatFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/matlab_format_export_on_non_windows.out")));
#endif
}

//==============================================================================

void Tests::exportToCellml10Tests()
{
    // Try to export a CellML 1.0 file to CellML 1.0

    QString fileName = OpenCOR::fileName("models/noble_model_1962.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "cellml_1_0"),
             QStringList() << "The file must be a CellML 1.1 file." << QString());

    // Export a CellML 1.1 file to CellML 1.0

    fileName = OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "cellml_1_0"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/cellml_1_0_export.out")) << QString());

    // Try to export a non-existing CellML file to CellML 1.0

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_file" << "cellml_1_0"),
             QStringList() << "The file could not be found." << QString());
}

//==============================================================================

void Tests::validateCellmlFiles()
{
    // Validate a valid CellML file

    QString fileName = OpenCOR::fileName("models/noble_model_1962.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << fileName),
             QStringList() << QString());

    // Try to validate some invalid CellML files

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/representation_error_issue.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << fileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/representation_error_issue.cellml.out")));

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_warning_issues.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << fileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_warning_issues.cellml.out")));

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_error_and_warning_issues.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << fileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_error_and_warning_issues.cellml.out")));

    // Try to validate a non-CellML file

    fileName = OpenCOR::fileName("models/tests/sedml/noble_1962_local.sedml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << fileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/noble_1962_local.sedml.out")));

    // Try to validate a non-existing CellML file

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::validate" << "non_existing_file"),
             QStringList() << "The file could not be found." << QString());
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
