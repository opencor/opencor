import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *


def test_data_store_values(values, name, indent=''):
    print('%s - Test %s:' % (indent, name))
    print('%s    - Size: %d' % (indent, len(values)))

    for uri, value in values.items():
        print('%s    - %s:' % (indent, uri))
        print('%s       - URI: %s' % (indent, value.uri()))
        print('%s       - Value: %f' % (indent, value.value()))
        test_value = 123.456789
        value.set_value(test_value)
        print('%s       - Test value properly set: %s' % (indent, "yes" if value.value() == test_value else "no"))


def test_simulation_data_property(get_method, set_method, description):
    orig_value = get_method()
    test_value = 123.456789

    print('       - %s: %f' % (description, orig_value))

    set_method(test_value)

    print('       - Test %s properly set: %s' % (description, "yes" if get_method() == test_value else "no"))

    set_method(orig_value)


def test_data_store_variable_index(variable, index, indent):
    print('%s    - value(%d): %f' % (indent, index, variable.value(index)))

    for run in range(variable.runs_count() + 3):
        print('%s    - value(%d, %d): %f' % (indent, index, run - 2, variable.value(index, run - 2)))


def test_data_store_variable(variable, name, indent=''):
    print('%s - Test %s:' % (indent, name))
    print('%s    - Name: %s' % (indent, variable.name()))
    print('%s    - Unit: %s' % (indent, variable.unit()))
    print('%s    - URI: %s' % (indent, variable.uri()))

    values_count = variable.values_count()

    test_data_store_variable_index(variable, -1, indent)
    test_data_store_variable_index(variable, 0, indent)
    test_data_store_variable_index(variable, values_count - 1, indent)
    test_data_store_variable_index(variable, values_count, indent)

    for run in range(variable.runs_count() + 3):
        print('%s    - values(%d): ' % (indent, run - 2), end='')
        print_values(variable.values(run - 2))


def test_data_store_variables(variables, name, indent=''):
    print('%s - Test %s:' % (indent, name))
    print('%s    - Size: %d' % (indent, len(variables)))

    for uri, variable in variables.items():
        test_data_store_variable(variable, uri, indent + '   ')


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

    # Coverage tests for SimulationData

    header('SimulationData coverage tests', False)

    simulation = open_simulation('sedml/lorenz.sedml')

    data = simulation.data()

    test_data_store_values(data.constants(), 'SimulationData.constants()')
    test_data_store_values(data.states(), 'SimulationData.states()')
    test_data_store_values(data.rates(), 'SimulationData.rates()')
    test_data_store_values(data.algebraic(), 'SimulationData.algebraic()')

    test_simulation_data_property(data.starting_point, data.set_starting_point, 'Starting point')
    test_simulation_data_property(data.ending_point, data.set_ending_point, 'Ending point')
    test_simulation_data_property(data.point_interval, data.set_point_interval, 'Point interval')

    # Coverage tests for SimulationResults

    header('SimulationResults coverage tests', False)

    simulation.reset()
    simulation.run()

    results = simulation.results()

    test_data_store_variable(results.voi(), 'SimulationResults.voi()')
    test_data_store_variables(results.constants(), 'SimulationResults.constants()')
    test_data_store_variables(results.states(), 'SimulationResults.states()')
    test_data_store_variables(results.rates(), 'SimulationResults.rates()')
    test_data_store_variables(results.algebraic(), 'SimulationResults.algebraic()')

    print('    - Test SimulationResults.data_store():')

    data_store = results.data_store()

    test_data_store_variable(data_store.voi(), 'DataStore.voi()', '   ')
    test_data_store_variables(data_store.variables(), 'DataStore.variables()', '   ')
    test_data_store_variables(data_store.voi_and_variables(), 'DataStore.voi_and_variables()', '   ')

    oc.close_simulation(simulation)
