import os
import platform
import subprocess
import sys

import nbformat
from nbconvert.preprocessors import ExecutePreprocessor
from nbconvert.preprocessors.execute import CellExecutionError


def run_python(script_file):
#===========================

    # Set path to file script

    script_path = os.path.split(script_file)[0]

    # Read the Python source code

    try:
        with open(script_file) as f:
          source = f.read()
    except IOError:
        sys.exit('Cannot read file: {}'.format(script_file))

    # Create a new notebook with a single cell containing the
    # contents of the Python file

    args = ['import sys', 'args = []']
    for arg in sys.argv[1:]:
        args.append('args.append("{}")'.format(arg.replace('"', '\\"')))
    args.append('sys.argv = args')

    nb = nbformat.v4.new_notebook()
    nb.cells.append(nbformat.v4.new_code_cell(source='\n'.join(args)))
    nb.cells.append(nbformat.v4.new_code_cell(source=source))

    # Create an execution process that runs an OpenCOR kernel
    # and use it to run the notebook

    try:
        ep = ExecutePreprocessor(timeout=-1, kernel_name='opencor')
        ep.preprocess(nb, {'metadata': {'path': script_path + '/'}})
    except CellExecutionError as err:
        pass

    # Tracebacks may contain ANSI escape codes which Windows
    # by default doesn't recognise

    if platform.system() == 'Windows':
        subprocess.call('', shell=True)

    # Write any output from the Python code

    for output in nb.cells[1].outputs:
        if output.output_type == 'stream':
            if output.name == 'stdout':
                sys.stdout.write(output.text)
            elif output.name == 'stderr':
                sys.stderr.write(output.text)
        elif output.output_type == 'error':
            sys.stderr.write('\n'.join(output.traceback) + '\n')

    # Close output streams

    sys.stderr.close()
    sys.stdout.close()


def setup_paths():
#=================

    # Determine the location of OpenCOR from the Python executable
    # and add it to the head of the system PATH

    opsys = platform.system()

    if   opsys == 'Darwin':
        path = os.environ['PATH'].split(':')
        root = '/'.join(sys.executable.split('/')[:-6])
        path.insert(0, root)
        os.environ['PATH'] = ':'.join(path)

    elif opsys == 'Linux':
        path = os.environ['PATH'].split(':')
        root = '/'.join(sys.executable.split('/')[:-3])
        path.insert(0, '%s/bin' % root)
        os.environ['PATH'] = ':'.join(path)

    elif opsys == 'Windows':
        path = os.environ['PATH'].split(';')
        root = '\\'.join(sys.executable.split('\\')[:-3])
        path.insert(0, '%s\\bin' % root)
        os.environ['PATH'] = ';'.join(path)


def main():
#==========
    if len(sys.argv) < 2:
        sys.exit('Usage: {} PYTHON_FILE'.format(sys.argv[0]))

    setup_paths()

    run_python(sys.argv[1])


if __name__ == '__main__':
#=========================
    main()
