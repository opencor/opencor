#!/usr/bin/env python
"""
    Based on `move-virtualenv`, updated for Python 3 and Windows with
    activation script processing removed.

    =======================================================================

    move-virtualenv`, a helper script that moves virtualenvs to a new
    location. https://github.com/fireteam/virtualenv-tools.

    Copyright (c) 2012 by Fireteam Ltd., see AUTHORS for more details.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.

        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.

        * The names of the contributors may not be used to endorse or
          promote products derived from this software without specific
          prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    =======================================================================
"""
import os
import re
import sys
import marshal
import optparse
import subprocess
from types import CodeType

python_exe = 'python.exe' if (os.name == 'nt') else 'python'
path_slash = '\\' if (os.name == 'nt') else '/'

bin_python = path_slash + 'bin' + path_slash + python_exe

_pybin_match = re.compile(r'^python\d+\.\d+$')


def update_script(script_filename, new_path):
    """Updates shebang lines for actual scripts."""
    try:
        with open(script_filename, 'r') as f:
            lines = list(f)
    except UnicodeDecodeError:
        return
    if not lines:
        return

    if not lines[0].startswith('#!'):
        return
    args = lines[0][2:].strip().split()
    if not args:
        return

    import pdb; pdb.set_trace()

    if not args[0].endswith(bin_python) \
    or '/usr/bin/env python' in args[0]:
        return

    new_bin = os.path.join(new_path, 'bin', python_exe)
    if new_bin == args[0]:
        return

    args[0] = new_bin
    lines[0] = '#!%s\n' % ' '.join(args)
    print('S %s' % script_filename)
    with open(script_filename, 'w') as f:
        f.writelines(lines)


def update_scripts(bin_dir, new_path):
    """Updates all scripts in the bin folder."""
    for fn in os.listdir(bin_dir):
        update_script(os.path.join(bin_dir, fn), new_path)


def update_pyc(filename, new_path):
    """Updates the filenames stored in pyc files."""
    print("PYCL %s" % filename)
    with open(filename, 'rb') as f:
        if sys.version_info < (3, 3): magic = f.read(8)
        else:                         magic = f.read(12)
        code = marshal.loads(f.read())

    def _make_code(code, filename, consts):
        return CodeType(code.co_argcount, code.co_kwonlyargcount, code.co_nlocals,
                        code.co_stacksize, code.co_flags, code.co_code,
                        tuple(consts), code.co_names, code.co_varnames, filename,
                        code.co_name, code.co_firstlineno, code.co_lnotab,
                        code.co_freevars, code.co_cellvars)

    def _process(code):
        new_filename = new_path
        consts = []
        for const in code.co_consts:
            if type(const) is CodeType:
                const = _process(const)
            consts.append(const)
        if new_path != code.co_filename or consts != list(code.co_consts):
            code = _make_code(code, new_path, consts)
        return code

    new_code = _process(code)

    if new_code is not code:
        print('B %s' % filename)
        with open(filename, 'wb') as f:
            f.write(magic)
            marshal.dump(new_code, f)


def update_pycs(lib_dir, new_path, lib_name):
    """Walks over all pyc files and updates their paths."""
    files = []

    def get_new_path(filename):
        filename = os.path.normpath(filename)
        if filename.startswith(lib_dir.rstrip('/') + '/'):
            return os.path.join(new_path, filename[len(lib_dir) + 1:])

    for dirname, dirnames, filenames in os.walk(lib_dir):
        for filename in filenames:
            if filename.endswith(('.pyc', '.pyo')):
                filename = os.path.join(dirname, filename)
                local_path = get_new_path(filename)
                if local_path is not None:
                    update_pyc(filename, local_path)


def update_local(base, new_path):
    """On some systems virtualenv seems to have something like a local
    directory with symlinks.  It appears to happen on debian systems and
    it causes havok if not updated.  So do that.
    """
    local_dir = os.path.join(base, 'local')
    if not os.path.isdir(local_dir):
        return

    for folder in 'bin', 'lib', 'include':
        filename = os.path.join(local_dir, folder)
        target = '../%s' % folder
        if os.path.islink(filename) and os.readlink(filename) != target:
            os.remove(filename)
            os.symlink('../%s' % folder, filename)
            print('L %s' % filename)


def update_paths(base, new_path):
    """Updates all paths in a virtualenv to a new one."""
    if new_path == 'auto':
        new_path = os.path.abspath(base)
    if not os.path.isabs(new_path):
        print('error: %s is not an absolute path' % new_path)
        return False

    bin_dir = os.path.join(base, 'bin')
    lib_dir = None
    lib_name = None

    if os.name == 'nt':
        scripts_dir = os.path.join(base, 'Scripts')
        base_lib_dir = base
        lib_name = 'Lib'
    else:
        scripts_dir = bin_dir
        base_lib_dir = os.path.join(base, 'lib')
        if os.path.isdir(base_lib_dir):
            for folder in os.listdir(base_lib_dir):
                if _pybin_match.match(folder):
                    lib_name = folder
                    break

    if lib_name:
        lib_dir = os.path.join(base_lib_dir, lib_name)

    if (lib_dir is None
     or not os.path.isdir(scripts_dir)
     or not os.path.isdir(bin_dir)
     or not os.path.isfile(os.path.join(bin_dir, python_exe))):
        print('error: %s does not refer to a python installation' % base)
        return False

    update_scripts(scripts_dir, new_path)
    update_pycs(lib_dir, new_path, lib_name)
    update_local(base, new_path)

    return True


def main():
    parser = optparse.OptionParser()
    parser.add_option('--update-path', help='Update the path for all '
                      'required executables and helper files that are '
                      'supported to the new python prefix.  You can also set '
                      'this to "auto" for autodetection.')
    options, paths = parser.parse_args()
    if not paths:
        paths = ['.']

    rv = 0
# Usage if no args...
    if options.update_path:
        for path in paths:
            if not update_paths(path, options.update_path):
                rv = 1
    sys.exit(rv)


if __name__ == '__main__':
    main()
