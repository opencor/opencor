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
// SBML API tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "sbmlapibegin.h"
    #include "sbml/common/libsbml-version.h"
    #include "sbml/SBMLDocument.h"
#include "sbmlapiend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to libSBML

    // Check the version of libSBML

    QCOMPARE(libsbml::getLibSBMLDottedVersion(), "5.13.0");

    // Check against which libraries libSBML has been compiled

    QVERIFY( libsbml::isLibSBMLCompiledWith("bzip2"));
    QVERIFY(!libsbml::isLibSBMLCompiledWith("expat"));
    QVERIFY( libsbml::isLibSBMLCompiledWith("libxml"));
    QVERIFY(!libsbml::isLibSBMLCompiledWith("xerces-c"));
    QVERIFY( libsbml::isLibSBMLCompiledWith("zip"));

    // Create an SBML document with a model inside it, then set the name of the
    // model and check that it has been properly set

    libsbml::SBMLDocument *sbmlDocument = new libsbml::SBMLDocument();
    libsbml::Model *sbmlModel = sbmlDocument->createModel();

    static const std::string ModelName = "myModel";

    sbmlModel->setName(ModelName);

    QCOMPARE(sbmlModel->getName(), ModelName);

    delete sbmlDocument;
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
