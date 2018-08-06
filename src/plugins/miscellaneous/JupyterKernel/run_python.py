#!/Users/dave/build/OpenCOR-2018-06-22-macOS/OpenCOR.app/Contents/Frameworks/Python/bin/python

import os
import sys

import nbformat
from nbconvert.preprocessors import ExecutePreprocessor


def main():
#==========

    if len(sys.argv) < 2:
        sys.exit('Usage: {} PYTHON_FILE'.format(sys.argv[0]))

    # Initialise path and file names

    source_file = sys.argv[1]
    script_path = os.path.split(source_file)[0]

    # Read the Python source code

    try:
        with open(source_file) as f:
          source = f.read()
    except IOError:
        sys.exit('Cannot read file: {}'.format(source_file))

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

    ep = ExecutePreprocessor(timeout=-1, kernel_name='opencor')
    ep.preprocess(nb, {'metadata': {'path': script_path + '/'}})

    # Write any output from the Python code

    for output in nb.cells[1].outputs:
        if output.name == 'stdout':
            sys.stdout.write(output.text)
        elif output.name == 'stderr':
            sys.stderr.write(output.text)

    # Close output streams

    sys.stderr.close()
    sys.stdout.close()


def setup_paths():
#=================
    pass
#### Windows
### Get OpenCOR's root directory
##root = '\\'.join(sys.executable.split('\\')[:-3])
### Add the location of our binaries to the head of the system PATH
##path = os.environ['PATH'].split(';')
##path.insert(0, '%s\\bin' % root)
##path.insert(0, '%s\\Python\\Scripts' % root)
##path.insert(0, '%s\\Python\\bin' % root)
##os.environ['PATH'] = ';'.join(path)


if __name__ == '__main__':
    setup_paths()
    main()
