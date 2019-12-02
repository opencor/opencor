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
// clcache wrapper
//==============================================================================

#include <stdio.h>
#include <stdlib.h>

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Call clcache with the given arguments, except the first one, which is the
    // full path to the MSVC compiler

    #define STRING_SIZE 32768

    char clcacheCommand[STRING_SIZE] = "clcache";
    int k = 6;

    for (int i = 2; i < pArgC; ++i) {
        clcacheCommand[++k] = ' ';

        for (int j = 0; pArgV[i][j]; ++j) {
            if (pArgV[i][j] == '\\') {
                clcacheCommand[++k] = '\\';
                clcacheCommand[++k] = '\\';
            } else if (pArgV[i][j] == '"') {
                clcacheCommand[++k] = '\\';
                clcacheCommand[++k] = '"';
            } else {
                clcacheCommand[++k] = pArgV[i][j];
            }
        }
    }

    clcacheCommand[++k] = '\0';

    return system(clcacheCommand);
}

//==============================================================================
// End of file
//==============================================================================
