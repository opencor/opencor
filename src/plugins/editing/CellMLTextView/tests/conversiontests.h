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
// CellML Text view conversion tests
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

class ConversionTests : public QObject
{
    Q_OBJECT

private slots:
    void successfulConversionTests();
    void failingConversionTests01();
    void failingConversionTests02();
    void failingConversionTests03();
    void failingConversionTests04();
    void failingConversionTests05();
    void failingConversionTests06();
    void failingConversionTests07();
    void failingConversionTests08();
    void failingConversionTests09();
    void failingConversionTests10();
    void failingConversionTests11();
    void failingConversionTests12();
    void failingConversionTests13();
    void failingConversionTests14();
    void failingConversionTests15();
    void failingConversionTests16();
    void warningConversionTests();
};

//==============================================================================
// End of file
//==============================================================================
