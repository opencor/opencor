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
// SED-ML Editing view interface
//==============================================================================

#include "interfacebegin.h"
    // Note: make sure to update sedmlEditingViewInterfaceVersion() whenever
    //       you update this interface...

    VIRTUAL void reformat(const QString &pFileName) const PURE_OR_OVERRIDE;

    VIRTUAL bool validSedml(const QString &pFileName,
                            QString &pExtra) const PURE_OR_OVERRIDE;
#include "interfaceend.h"

//==============================================================================
// End of file
//==============================================================================
