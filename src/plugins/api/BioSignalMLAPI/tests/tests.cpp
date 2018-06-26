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
// BioSignalML API tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "biosignalml/biosignalml.h"
#include "typedobject/typedobject.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to BioSignalML

    // Check the version of BioSignalML and typedobject

    QCOMPARE(bsml::VERSION.c_str(), "0.8.4");
    QCOMPARE(tobj::VERSION.c_str(), "1.1.2");
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
