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
// SEDML API tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/common/libsedml-version.h"
    #include "sedml/SedDocument.h"
#include "sedmlapiend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to libSEDML

    // Check the version of libSEDML

    QCOMPARE(libsedml::getLibSEDMLDottedVersion(), "0.3.1");

    // Create a SED-ML document with a model inside it, then set the name of the
    // model and check that it has been properly set

    libsedml::SedDocument *sedmlDocument = new libsedml::SedDocument();
    libsedml::SedModel *sedmlModel = sedmlDocument->createModel();

    static const std::string ModelName = "myModel";

    sedmlModel->setName(ModelName);

    QCOMPARE(sedmlModel->getName(), ModelName);

    delete sedmlDocument;
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
