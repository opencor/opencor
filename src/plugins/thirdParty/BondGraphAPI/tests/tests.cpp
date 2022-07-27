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
// libSBML tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "bondgraphapibegin.h"
    #include "bondgraph.hpp"
#include "bondgraphapiend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to libBondGraph
    //Create and test a simple bondgraph
    BG::newWorkSpace();
    auto ioBondGraph = BG::createBondGraph();

    //Create the resistor
    auto lR = BG::createResistor();
    lR->setParameter("r", "1", "Ohm");
    ioBondGraph->addComponent(lR);

    auto lC1 = BG::createCapacitor();
    lC1->setParameter("C", "1", "Farad");
    lC1->setParameter("q_", "1", "coulomb");
    ioBondGraph->addComponent(lC1);

    //Create the junctions
    auto lJ0_1 = BG::createOneJunction();
    ioBondGraph->addComponent(lJ0_1);

    //Create the bonds
    ioBondGraph->connect(lR, lJ0_1);
    ioBondGraph->connect(lC1, lJ0_1);

    auto res = ioBondGraph->computeStateEquation();
    // Solvable
    QCOMPARE(res.bondGraphValidity, true);
    auto equations = res.dof;
    std::map<std::string, std::string> result = {
        {"q_1", "u_3*C_1"},
        {"dot_Inductor", "u_3"},
        {"dot_Capacitor", "C_1*dot_u_3"}};

    for (auto eq : equations) {
        std::string svar = BG::symEngineExpressionToString(eq.first);
        std::string sres = BG::symEngineExpressionToString(eq.second);
        std::string expected = result[svar];
        QCOMPARE(sres, expected);
    }

}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
