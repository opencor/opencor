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

    QCOMPARE(libsbml::getLibSBMLDottedVersion(), "5.15.0");

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
