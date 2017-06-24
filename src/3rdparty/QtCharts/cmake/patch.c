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
// QtCharts CMake file patcher
//==============================================================================

#include <stdio.h>
#include <string.h>

//==============================================================================

FILE *in;
FILE *out;
char outFileName[FILENAME_MAX];

//==============================================================================

int cleanUp()
{
    if (in)
        fclose(in);

    if (out) {
        fclose(out);

        remove(outFileName);
    }

    return 0;
}

//==============================================================================

int error(char *pErrorMessage)
{
    // Output the given error message and free/close a few things

    fprintf(stderr, "%s", pErrorMessage);

    cleanUp();

    return 1;
}

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    #define LINE_MAX 1024

    char line[LINE_MAX];

    // Make sure that a file name has been provided

    if (pArgC != 2) {
        printf("Usage: %s <filename>\n", pArgV[0]);

        return 1;
    }

    // Open the file

    in = fopen(pArgV[1], "r");
    out = 0;

    if (!in)
        return error("The file could not be opened.\n");

    // Open a temporary file

    sprintf(outFileName, "%s.tmp", pArgV[1]);

    out = fopen(outFileName, "w");

    if (!out)
        return error("The temporary file could not be created.\n");

    // Process each line of the file and output it to our temporary file

    while (fgets(line, LINE_MAX, in)) {
        if (!strcmp(line, "get_filename_component(_qt5Charts_install_prefix \"${CMAKE_CURRENT_LIST_DIR}/../../../\" ABSOLUTE)\n"))
            fprintf(out, "set(_qt5Charts_install_prefix ${CMAKE_SOURCE_DIR}/src/3rdparty/QtCharts/ext)\n");
        else
            fprintf(out, "%s", line);
    }

    fclose(in);
    fclose(out);

    in = out = 0;

    remove(pArgV[1]);

    if (rename(outFileName, pArgV[1]) == -1)
        return error("The file could not be patched.\n");

    // We are all done

    return 0;
}
