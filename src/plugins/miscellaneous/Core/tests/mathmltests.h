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

private slots:
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
