import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *


def edge_test_data_store_variable(variable):
    print('          - Name: %s' % variable.name())
    print('          - Unit: %s' % variable.unit())
    print('          - URI: %s' % variable.uri())
    print('          - value(-1): %f' % variable.value(-1))
    print('          - value(-1, -2): %f' % variable.value(-1, -2))
    print('          - value(-1, -1): %f' % variable.value(-1, -1))
    print('          - value(-1, 0): %f' % variable.value(-1, 0))
    print('          - value(-1, 1): %f' % variable.value(-1, 1))
    print('          - value(0): %f' % variable.value(0))
    print('          - value(0, -2): %f' % variable.value(0, -2))
    print('          - value(0, -1): %f' % variable.value(0, -1))
    print('          - value(0, 0): %f' % variable.value(0, 0))
    print('          - value(0, 1): %f' % variable.value(0, 1))
    print('          - value(50000): %f' % variable.value(50000))
    print('          - value(50000, -2): %f' % variable.value(50000, -2))
    print('          - value(50000, -1): %f' % variable.value(50000, -1))
    print('          - value(50000, 0): %f' % variable.value(50000, 0))
    print('          - value(50000, 1): %f' % variable.value(50000, 1))
    print('          - value(50001): %f' % variable.value(50001))
    print('          - value(50001, -2): %f' % variable.value(50001, -2))
    print('          - value(50001, -1): %f' % variable.value(50001, -1))
    print('          - value(50001, 0): %f' % variable.value(50001, 0))
    print('          - value(50001, 1): %f' % variable.value(50001, 1))


def edge_test_simulation_results(simulation):
    print('    - Test the SimulationResults class:')

    results = simulation.results()

    print('       - Test SimulationResults.voi():')

    edge_test_data_store_variable(results.voi())


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

    # Test various edge cases using a valid SED-ML file

    header('Various edge cases', False)

    simulation = open_simulation('sedml/lorenz.sedml')

    print(' - Run simulation:')

    simulation.run()

    edge_test_simulation_results(simulation)

    oc.close_simulation(simulation)
