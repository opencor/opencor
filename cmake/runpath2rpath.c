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
// RUNPATH to RPATH converter
//==============================================================================

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//==============================================================================

int fileDescriptor;
void *dynamicSection;

//==============================================================================

int cleanUp()
{
    free(dynamicSection);

    close(fileDescriptor);

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
    int i;
    Elf64_Ehdr elfHeader;
    Elf64_Phdr programHeader;
    int hasRunpathTag;
    int runpathTagIndex;

    // Make sure that a file name has been provided

    if (pArgC != 2) {
        printf("Usage: %s <filename>\n", pArgV[0]);

        return 1;
    }

    // Open the file

    fileDescriptor = open(pArgV[1], O_RDWR);
    dynamicSection = 0;

    if (fileDescriptor == -1)
        return error("The file could not be opened.\n");

    if (read(fileDescriptor, &elfHeader, EI_NIDENT) != EI_NIDENT)
        return error("The ELF header (identifier) could not be read.\n");

    if (   memcmp(elfHeader.e_ident, ELFMAG, SELFMAG)
        || (elfHeader.e_ident[EI_CLASS] != ELFCLASS64)
        || (elfHeader.e_ident[EI_DATA] != ELFDATA2LSB)
        || (elfHeader.e_ident[EI_VERSION] != EV_CURRENT)) {
        return error("The file is probably not an ELF file.\n");
    }

    if (read(fileDescriptor, ((char *) &elfHeader)+EI_NIDENT, sizeof(Elf64_Ehdr)-EI_NIDENT) != (ssize_t) (sizeof(Elf64_Ehdr)-EI_NIDENT))
        return error("The ELF header could not be read.\n");

    if ((size_t) elfHeader.e_phentsize != sizeof(Elf64_Phdr))
        return error("The program header size is wrong.\n");

    // Check that we have a dynamic section

    if (lseek(fileDescriptor, elfHeader.e_phoff, SEEK_SET) == -1)
        return error("The program header could not be found.\n");

    for (i = 0; i < elfHeader.e_phnum; ++i) {
        if (read(fileDescriptor, &programHeader, sizeof(Elf64_Phdr)) != (ssize_t) sizeof(Elf64_Phdr))
            return error("The dynamic section header could not be read.\n");

        if (programHeader.p_type == PT_DYNAMIC)
            break;
    }

    if (i == elfHeader.e_phnum)
        return error("No dynamic section was found.\n");

    if (!programHeader.p_filesz)
        return error("The dynamic section is empty.\n");

    // Allocate some memory for the dynamic section and retrieve it

    dynamicSection = malloc(programHeader.p_filesz);

    if (!dynamicSection)
        return error("Memory for the dynamic section could not be allocated.\n");

    memset(dynamicSection, 0, programHeader.p_filesz);

    if (lseek(fileDescriptor, programHeader.p_offset, SEEK_SET) == -1)
        return error("The dynamic section could not be found.\n");

    if (read(fileDescriptor, dynamicSection, programHeader.p_filesz) != (ssize_t) programHeader.p_filesz)
        return error("The dynamic section could not be read.\n");

    // Retrieve the index of the RUNPATH tag

    hasRunpathTag = 0;

    for (runpathTagIndex = 0; (i = ((Elf64_Dyn *) dynamicSection)[runpathTagIndex].d_tag) != DT_NULL; ++runpathTagIndex) {
        if (i == DT_RUNPATH) {
            hasRunpathTag = 1;

            break;
        }
    }

    if (!hasRunpathTag)
        return cleanUp();

    // Convert the RUNPATH value to a RPATH value

    ((Elf64_Dyn *) dynamicSection)[runpathTagIndex].d_tag = DT_RPATH;

    if (lseek(fileDescriptor, programHeader.p_offset, SEEK_SET) == -1)
        return error("The RUNPATH value could not be found.\n");

    if (write(fileDescriptor, dynamicSection, programHeader.p_filesz) != (int) programHeader.p_filesz)
        return error("The RUNPATH value could not be converted to a RPATH value.\n");

    // We are all done, so clean up and leave

    return cleanUp();
}
