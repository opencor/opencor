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
// libSBML tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "sbml/common/libsbml-version.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to libSBML

    // Check the version of libSBML

    QCOMPARE(libsbml::getLibSBMLDottedVersion(), "5.11.4");

    // Check aginast which libraries libSBML has been compiled

    QVERIFY( libsbml::isLibSBMLCompiledWith("bzip2"));
    QVERIFY(!libsbml::isLibSBMLCompiledWith("expat"));
    QVERIFY( libsbml::isLibSBMLCompiledWith("libxml"));
    QVERIFY(!libsbml::isLibSBMLCompiledWith("xerces-c"));
    QVERIFY( libsbml::isLibSBMLCompiledWith("zip"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
