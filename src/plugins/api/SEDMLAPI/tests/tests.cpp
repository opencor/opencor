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
