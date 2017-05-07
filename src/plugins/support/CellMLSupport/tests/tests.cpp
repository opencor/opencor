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
// CellML support tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::doRuntimeTest(const QString &pFileName,
                          const QString &pCellmlVersion,
                          const QStringList &pModelParameters,
                          const bool &pIsValid)
{
    // Get a CellML file object for the given CellML file and make sure that it
    // is considered of the expected CellML version

    OpenCOR::CellMLSupport::CellmlFile cellmlFile(pFileName);

    QCOMPARE(QString::fromStdWString(cellmlFile.model()->cellmlVersion()), pCellmlVersion);

    // Retrieve a runtime for our model and make sure that it is valid

    OpenCOR::CellMLSupport::CellmlFileRuntime *runtime = cellmlFile.runtime();

    QVERIFY(runtime);
    QVERIFY(pIsValid?runtime->isValid():!runtime->isValid());

    if (!pIsValid)
        return;

    // Retrieve the model parameters (i.e. the ones that would be shown in, say,
    // the Simulation Experiment view) for our model and check that they contain
    // the information we would expect

    QStringList modelParameters = QStringList();
    OpenCOR::CellMLSupport::CellmlFileRuntimeParameters parameters = runtime->parameters();

    foreach (OpenCOR::CellMLSupport::CellmlFileRuntimeParameter *parameter, parameters) {
        modelParameters << QString("%1 (%2)").arg(parameter->fullyFormattedName(),
                                                  parameter->formattedUnit(runtime->variableOfIntegration()->unit()));
    }

    QCOMPARE(modelParameters << QString(), pModelParameters);
}

//==============================================================================

void Tests::runtimeTests()
{
    // Run some runtime-related tests on the Noble 1962 model

    QStringList modelParameters = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/noble_model_1962.out"));

    doRuntimeTest(OpenCOR::fileName("models/noble_model_1962.cellml"), "1.0",
                  modelParameters);

    // Do the same as above but using the CellML 1.1 namespace
    // Note: the idea is to check that what we are doing to retrieve the
    //       parameters of a model is not dependent on the version of a CellML
    //       file...

    QString fileName = OpenCOR::Core::temporaryFileName();
    QString fileContents = OpenCOR::rawFileContents(OpenCOR::fileName("models/noble_model_1962.cellml"));

    fileContents.replace("cellml/1.0#", "cellml/1.1#");

    QVERIFY(OpenCOR::Core::writeFileContentsToFile(fileName, fileContents));

    doRuntimeTest(fileName, "1.1", modelParameters);

    // Now, we do the same for some proper CellML 1.1 models:
    //  - Hodgking-Huxley model, which is somewhat 'complex' in terms of
    //    imports, etc.;
    //  - An 'old' version of a bond graph model implementation where the
    //    variable of integration is not visible in the main CellML file (so the
    //    idea is to ensure that the model is still considered valid even though
    //    the variable of integration is not directly visible);
    //  - A 'new' version of a bond graph model implementation where the
    //    variable of integration is now visible in the main CellML file (as
    //    well as some other model parameters); and
    //  - A model (Noble 1962) that imports its units (and no components) from a
    //    child model; and
    //  - A somewhat comprehensive model (Faville 2008).

    doRuntimeTest(OpenCOR::fileName("doc/developer/functionalTests/res/cellml/cellml_1_1/experiments/periodic-stimulus.xml"),
                  "1.1", OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/periodic-stimulus.out")));
    doRuntimeTest(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/bond_graph_model_old.cellml"),
                  "1.1", OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/bond_graph_model_old.out")));
    doRuntimeTest(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/bond_graph_model_new.cellml"),
                  "1.1", OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/bond_graph_model_new.out")));
    doRuntimeTest(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/units_import_only_parent_model.cellml"),
                  "1.1", modelParameters);
    doRuntimeTest(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/faville_model_2008.cellml"),
                  "1.1", OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/faville_model_2008.out")));

    // Finally, test a CellML file that has, according to the CellML API at
    // least, several variables of integration

    doRuntimeTest(OpenCOR::fileName("src/plugins/support/CellMLSupport/tests/data/calcium transient.cellml"),
                  "1.1", QStringList(), false);

    // Clean up after ourselves

    QFile::remove(fileName);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
