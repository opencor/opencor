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
// Architecture
//==============================================================================

#include <stdio.h>

//==============================================================================

int main()
{
    // Print out the architecture, if recognised

    switch (sizeof(void *)) {
    case 4:
        printf("32");

        break;
    case 8:
        printf("64");

        break;
    default:
        printf("Unknown");
    }

    return 0;
}

//==============================================================================
// End of file
//==============================================================================
