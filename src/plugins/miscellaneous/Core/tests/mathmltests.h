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
// Core MathML tests
//==============================================================================

#pragma once

//==============================================================================

#include <QAbstractMessageHandler>
#include <QObject>
#include <QStringList>

//==============================================================================

class DummyMessageHandler : public QAbstractMessageHandler
{
    Q_OBJECT

protected:
    virtual void handleMessage(QtMsgType pType, const QString &pDescription,
                               const QUrl &pIdentifier,
                               const QSourceLocation &pSourceLocation);
};

//==============================================================================

class MathmlTests : public QObject
{
    Q_OBJECT

private:
    void tests(const QString &pCategory);

private Q_SLOTS:
    void initTestCase();

    void plusTests();
    void minusTests();
    void timesTests();
    void divideTests();

    void powerTests();
    void rootTests();

    void absTests();

    void expTests();
    void lnTests();
    void logTests();

    void floorTests();
    void ceilTests();

    void factTests();

    void remTests();

    void minTests();
    void maxTests();

    void gcdTests();
    void lcmTests();

    void trigonometricTests();
};

//==============================================================================
// End of file
//==============================================================================
