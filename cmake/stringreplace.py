################################################################################
#
# Copyright (C) The University of Auckland
#
# OpenCOR is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# OpenCOR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################

################################################################################
#
# String replace
#
################################################################################

#!/usr/bin/env python

################################################################################

import sys

################################################################################

if __name__ == "__main__":
    # Make sure that a file name and two strings have been provided

    if len(sys.argv) != 4:
        print("Usage: "+sys.argv[0]+" <filename> <fromString> <toString>")

        exit()

    # Retrieve the contents of the given file

    with open(sys.argv[1]) as file:
        fileContents = file.read()

    # Update the contents of the given file

    with open(sys.argv[1], "w") as file:
        file.write(fileContents.replace(sys.argv[2], sys.argv[3]))

################################################################################
# End of file
################################################################################
