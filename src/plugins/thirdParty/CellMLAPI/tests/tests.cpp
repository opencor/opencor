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
// CellML API tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "cellmlapibegin.h"
    #include "cellml-api-cxx-support.hpp"

    #include "IfaceCellML_APISPEC.hxx"

    #include "CellMLBootstrap.hpp"
#include "cellmlapiend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to the CellML API

    // Get a bootstrap object and its model loader

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();
    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

    QVERIFY(cellmlBootstrap);
    QVERIFY(modelLoader);

    // Create a CellML 1.0 model

    ObjRef<iface::cellml_api::Model> cellml10Model = cellmlBootstrap->createModel(L"1.0");

    QVERIFY(cellml10Model);

    // Create a CellML 1.1 model

    ObjRef<iface::cellml_api::Model> cellml11Model = cellmlBootstrap->createModel(L"1.1");

    QVERIFY(cellml11Model);

    // Create an invalid CellML model

    ObjRef<iface::cellml_api::Model> invalidCellmlModel;

    try {
        invalidCellmlModel = cellmlBootstrap->createModel(L"xxx");
    } catch (...) {
        QVERIFY(!invalidCellmlModel);
    }

    // Load an existing model

    ObjRef<iface::cellml_api::Model> existingModel = modelLoader->loadFromURL(QUrl::fromPercentEncoding(QUrl::fromLocalFile(QFileInfo(OpenCOR::fileName("models/noble_model_1962.cellml")).canonicalFilePath()).toEncoded()).toStdWString());

    QVERIFY(existingModel);

    QCOMPARE(QString::fromStdWString(existingModel->name()), QString("noble_model_1962"));
    QCOMPARE(QString::fromStdWString(existingModel->cellmlVersion()), QString("1.0"));

    // Load a non-existing model

    ObjRef<iface::cellml_api::Model> nonExistingModel;

    try {
        nonExistingModel = modelLoader->loadFromURL(L"xxx");
    } catch (...) {
        QVERIFY(!nonExistingModel);
    }
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
