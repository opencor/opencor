# Copyright (C) The University of Auckland

# OpenCOR is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# OpenCOR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://gnu.org/licenses>.

import os
import re
import sys

if __name__ == '__main__':
    for filename in os.listdir(sys.argv[1]):
        # Read the contents of the given file

        full_filename = os.path.join(sys.argv[1], filename)

        try:
            with open(full_filename, 'r') as f:
                lines = list(f)
        except (IsADirectoryError, UnicodeDecodeError, PermissionError, OSError):
            continue

        # Check whether it's a Python script and, if so, change its shebang to
        # use our copy of Python, relative to this script
        # Note: see https://stackoverflow.com/a/62268465...

        if re.match('#!.*python', lines[0]):
            lines[0] = f'#!/usr/bin/perl -e$_=$ARGV[0];s/[^\/]+$/python/;exec($_,@ARGV)\n'

            with open(full_filename, 'w') as f:
                f.writelines(lines)
