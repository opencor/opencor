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
