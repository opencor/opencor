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
// CellML tools tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/help.out"));

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools" }, mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::help" }, mOutput));
    QCOMPARE(mOutput, help);

    // Try a known command, but with the wrong number of arguments

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::export", "argument" }, mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::validate", "argument", "argument" }, mOutput));
    QCOMPARE(mOutput, help);

    // Try an unknown command, resulting in the help being shown

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::unknown" }, mOutput));
    QCOMPARE(mOutput, help);
}

//==============================================================================

void Tests::exportToUnknownFormatOrLanguage()
{
    // Try to export a local file to an unknown format/language

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::export", OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml"), "unknown" }, mOutput ));
    QCOMPARE(mOutput, QStringList() << "The format or language is not valid." << QString());
}

//==============================================================================

void Tests::exportToCellml10Tests()
{
    // Try to export a CellML 1.0 file to CellML 1.0

    QString fileName = OpenCOR::fileName("models/noble_model_1962.cellml");

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::export", fileName, "cellml_1_0" }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file must be a CellML 1.1 file." << QString());

    // Export a CellML 1.1 file to CellML 1.0

    fileName = OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", fileName, "cellml_1_0" }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/cellml_1_0_export.out")) << QString());

    // Export a CellML 1.1 file with an e-notation number to CellML 1.0

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/e_notation.cellml");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", fileName, "cellml_1_0" }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/e_notation_export.out")) << QString());

    // Export a CellML 1.1 file with duplicate ids to CellML 1.0

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/duplicate_ids/main.cellml");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", fileName, "cellml_1_0" }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/duplicate_ids/main.out")) << QString());

    // Try to export a non-existing CellML file to CellML 1.0

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::export", "non_existing_file", "cellml_1_0" }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be found." << QString());
}

//==============================================================================

void Tests::exportToCTests()
{
    // Try to export a local file to C

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml"), "c" }, mOutput ));
    QVERIFY(   (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.c.out")))
            || (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.c.alternative.out"))));
}

//==============================================================================

void Tests::exportToFortran77Tests()
{
    // Try to export a local file to FORTRAN 77

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml"), "fortran_77" }, mOutput ));
    QVERIFY(   (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.f.out")))
            || (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.f.alternative.out"))));
}

//==============================================================================

void Tests::exportToMatlabTests()
{
    // Try to export a local file to MATLAB

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml"), "matlab" }, mOutput ));
    QVERIFY(   (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.m.out")))
            || (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.m.alternative.out"))));
}

//==============================================================================

void Tests::exportToPythonTests()
{
    // Try to export a local file to Python

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::export", OpenCOR::fileName("models/tests/cellml/cellml_1_1/experiments/periodic-stimulus.xml"), "python" }, mOutput ));
    QVERIFY(   (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.py.out")))
            || (mOutput == OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/periodic-stimulus.py.alternative.out"))));
}

//==============================================================================

void Tests::validateCellmlFiles()
{
    // Validate a valid CellML file

    QString fileName = OpenCOR::fileName("models/noble_model_1962.cellml");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::validate", fileName }, mOutput));
    QCOMPARE(mOutput, QStringList() << QString());

    // Try to validate some CellML files with error and/or warning issues

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/representation_error_issue.cellml");

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::validate", fileName }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/representation_error_issue.cellml.out")));

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_warning_issues.cellml");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTools::validate", fileName }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_warning_issues.cellml.out")));

    fileName = OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_error_and_warning_issues.cellml");

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::validate", fileName }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/semantic_error_and_warning_issues.cellml.out")));

    // Try to validate a non-CellML file

    fileName = OpenCOR::fileName("models/tests/sedml/noble_1962_local.sedml");

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::validate", fileName }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tools/CellMLTools/tests/data/noble_1962_local.sedml.out")));

    // Try to validate a non-existing CellML file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTools::validate", "non_existing_file" }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be found." << QString());
}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
