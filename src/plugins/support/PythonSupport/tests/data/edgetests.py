import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *

if __name__ == '__main__':
    # Test for no file name or URL provided

    try:
        test_simulation('No file name or URL provided', '')
    except Exception as e:
        print(' - %s' % repr(e))

    # Test for an unknown local/remote file

    try:
        test_simulation('Unknown local file', 'unknown')
    except Exception as e:
        print(' - %s' % repr(e))

    try:
        test_simulation('Unknown remote file',
                        'https://unknown', False)
    except Exception as e:
        print(' - %s' % repr(e))

    # Test for an invalid local/remote file

    try:
        test_simulation('Invalid local file', 'cellml/underconstrained_model.cellml', False)
    except Exception as e:
        print(' - %s' % repr(e))

    try:
        test_simulation('Invalid remote file',
                        'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/underconstrained_model.cellml',
                        False)
    except Exception as e:
        print(' - %s' % repr(e))
