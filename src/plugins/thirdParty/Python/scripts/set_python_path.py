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
import logging
import marshal
import argparse
import subprocess
from types import CodeType
from collections import OrderedDict

windows = (os.name == 'nt')

python_exe = 'python.exe' if windows else 'python'
path_slash = '\\' if windows else '/'
bin_python = path_slash + 'bin' + path_slash + python_exe

_pybin_match = re.compile(r'^python\d+\.\d+$')

def update_script(script_filename, new_path, clear_args, extra_args):
    """Updates shebang lines for actual scripts."""
    try:
        with open(script_filename, 'r') as f:
            lines = list(f)
    except (IsADirectoryError, UnicodeDecodeError, PermissionError):
        return

    if not lines:
        return

    if not lines[0].startswith('#!'):
        return

    line = lines[0][2:]
    if line[0] in ['"', "'"]:
        has_quote = True
        quote = line[0]
        end = line[1:].index(quote) + 1
        args = [line[1:end]] + line[end+1:].split()
    else:
        has_quote = False
        quote = '"'
        args = line.strip().split()

    if not args:
        return

    if not args[0].endswith(bin_python) \
    or '/usr/bin/env python' in args[0]:
        return

    if clear_args: del args[1:]
    arg_set = OrderedDict([(a, None) for a in args[1:]])
    arg_set.update(OrderedDict([(a, None) for a in extra_args]))
    new_args = list(arg_set.keys())

    add_quote = (' ' in new_path)
    new_bin = os.path.join(new_path, 'bin', python_exe)
    if new_bin == args[0] and has_quote == add_quote and new_args == args[1:]:
        return

    if add_quote:
        args[0] = '%s%s%s' % (quote, new_bin, quote)
    else:
        args[0] = new_bin
    args[1:] = new_args

    logging.info('S: %s', script_filename)
    lines[0] = '#!%s\n' % ' '.join(args)
    with open(script_filename, 'w') as f:
        f.writelines(lines)


def update_scripts(bin_dir, new_path, clear_args, extra_args):
    """Updates all scripts in the bin folder."""
    for fn in os.listdir(bin_dir):
        update_script(os.path.join(bin_dir, fn), new_path, clear_args, extra_args)


def update_pyc(filename, new_path):
    """Updates the filenames stored in pyc files."""
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
        logging.info('B: %s', filename)
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
            filename = os.path.join(dirname, filename)
            if (filename.endswith(('.pyc', '.pyo'))
            and not os.path.dirname(filename).endswith('__pycache__')):
                local_path = get_new_path(filename)
                if local_path is not None:
                    update_pyc(filename, local_path)


def update_paths(base, scripts_dir, clear_args, extra_args):
    """Updates all paths in a virtualenv to a new one."""
    new_path = os.path.abspath(base)
    bin_dir = os.path.join(base, 'bin')
    lib_dir = None
    lib_name = None

    if scripts_dir == 'auto':
        if windows:
            scripts_dir = os.path.join(base, 'Scripts')
        else:
            scripts_dir = bin_dir

    if windows:
        base_lib_dir = base
        lib_name = 'Lib'
    else:
        base_lib_dir = os.path.join(base, 'lib')
        if os.path.isdir(base_lib_dir):
            for folder in os.listdir(base_lib_dir):
                if _pybin_match.match(folder):
                    lib_name = folder
                    break

    if (lib_name is None
     or not os.path.isdir(scripts_dir)
     or not os.path.isdir(bin_dir)
     or not os.path.isfile(os.path.join(bin_dir, python_exe))):
        print('error: %s does not refer to a Python installation' % base)
        return False

    lib_dir = os.path.join(base_lib_dir, lib_name)

    update_scripts(scripts_dir, new_path, clear_args, extra_args)
    update_pycs(lib_dir, new_path, lib_name)

    return True


def main():
    parser = argparse.ArgumentParser(description='Update the path of scripts '
                                                 'to a new Python prefix')

    parser.add_argument('-c', '--clear-args', dest='clear_args',
                        default=False, action='store_true',
                        help='Clear all existing arguments to Python')

    parser.add_argument('-u', '--update-path', dest='update_path',
                        help='Path to scripts. Set to "auto" for autodetection')

    parser.add_argument('-v', '--verbose', dest='verbose',
                        default=False, action='store_true',
                        help='Show names of updated scripts')

    parser.add_argument('path', metavar='PATH', help='Path to new Python installation.')

    parser.add_argument('extra_args', metavar='ARGS', nargs=argparse.REMAINDER,
                        help='Additional arguments to append to Python')

    args = parser.parse_args()
    if not args.update_path:
        args.update_path = 'auto'

    if not args.verbose:
        logging.disable(logging.INFO)

    sys.exit(0 if update_paths(args.path, args.update_path, args.clear_args, args.extra_args) else 1)


if __name__ == '__main__':
    main()
