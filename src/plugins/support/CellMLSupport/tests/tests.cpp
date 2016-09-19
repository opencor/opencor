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
                          const QStringList &pModelParameters)
{
    // Get a CellML file object for the given CellML file and make sure that it
    // is considered of the expected CellML version

    OpenCOR::CellMLSupport::CellmlFile cellmlFile(pFileName);

    QCOMPARE(QString::fromStdWString(cellmlFile.model()->cellmlVersion()), pCellmlVersion);

    // Retrieve a runtime for our model and make sure that it is valid

    OpenCOR::CellMLSupport::CellmlFileRuntime *runtime = cellmlFile.runtime();

    QVERIFY(runtime);
    QVERIFY(runtime->isValid());

    // Retrieve the model parameters (i.e. the ones that would be shown in, say,
    // the Single Cell view) for our model and check that they contain the
    // information we would expect

    QStringList modelParameters = QStringList();
    OpenCOR::CellMLSupport::CellmlFileRuntimeParameters parameters = runtime->parameters();

    foreach (OpenCOR::CellMLSupport::CellmlFileRuntimeParameter *parameter, parameters) {
        modelParameters << QString("%1 (%2)").arg(parameter->fullyFormattedName())
                                             .arg(parameter->formattedUnit(runtime->variableOfIntegration()->unit()));
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

    // Finally, we do the same for some proper CellML 1.1 models:
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

    // Clean up after ourselves

    QFile::remove(fileName);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
