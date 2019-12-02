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
// Silent run
//==============================================================================

#include <stdio.h>
#include <stdlib.h>

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Execute the given program, silencing its output

#ifdef _WIN32
    freopen("NUL:", "w", stdout);
    freopen("NUL:", "w", stderr);
#else
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
#endif

    #define STRING_SIZE 32768

    char command[STRING_SIZE];
    int k = -1;

    for (int i = 1; i < pArgC; ++i) {
        if (i != 1)
            command[++k] = ' ';

        for (int j = 0; pArgV[i][j]; ++j) {
            if (pArgV[i][j] == '\\') {
                command[++k] = '\\';
                command[++k] = '\\';
            } else if (pArgV[i][j] == '"') {
                command[++k] = '\\';
                command[++k] = '"';
            } else {
                command[++k] = pArgV[i][j];
            }
        }
    }

    command[++k] = '\0';

    return system(command);
}

//==============================================================================
// End of file
//==============================================================================
