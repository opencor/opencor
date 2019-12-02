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
// libNuML tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "libnumlbegin.h"
    #include "numl/common/libnuml-version.h"
    #include "numl/NUMLDocument.h"
#include "libnumlend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to libNuML

    // Check the version of libNuML

    QCOMPARE(libnuml::getLibNUMLDottedVersion(), "1.1.1");

    // Create a NuML document with a model inside it, then set the name of the
    // model and check that it has been properly set

    auto numlDocument = new libnuml::NUMLDocument();

    static const std::string ElementName = "numl";

    QCOMPARE(numlDocument->getElementName(), ElementName);

    delete numlDocument;
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
