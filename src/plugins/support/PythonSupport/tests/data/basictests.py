import math
import opencor as oc
import os


def header(title, first=True):
    if not first:
        print()

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')


def open_simulation(file_name_or_url):
    if file_name_or_url.startswith('https://'):
        return oc.open_simulation(file_name_or_url)
    else:
        original_directory = os.getcwd()

        os.chdir(os.path.dirname(__file__) + '/../../../../../../models/')

        simulation = oc.open_simulation(file_name_or_url)

        os.chdir(original_directory)

        return simulation


def print_values(data):
    if data is None:
        print('None')
    else:
        data_len = len(data)

        if data_len > 6:
            print('[ %f, %f, %f, ..., %f, %f, %f ]' %
                  (data[0], data[1], data[2], data[data_len - 3], data[data_len - 2],
                   data[data_len - 1]))
        else:
            print('[ ', end='')
            print(*data, sep=", ", end='')
            print(' ]')


def values(data, type, indent=''):
    if data:
        print('%s       - %s:' % (indent, type))

        for item in data.values():
            print('%s          - %s = ' % (indent, item.uri()), end='')

            try:
                print_values(item.values())
            except:
                print(item.value())
    else:
        print('%s       - %s: empty' % (indent, type))


def run_simulation(simulation, step):
    # Run the simulation

    print(' - Run simulation [%d]:' % step)
    print('    - Settings:')

    data = simulation.data()

    print('       - Starting point: %f' % data.starting_point())
    print('       - Ending point: %f' % data.ending_point())
    print('       - Point interval: %f' % data.point_interval())
    print('       - ODE solver: %s' % data.ode_solver_name())
    print('       - Initial values:')

    values(data.constants(), 'Constants', '   ')
    values(data.states(), 'States', '   ')
    values(data.rates(), 'Rates', '   ')
    values(data.algebraic(), 'Algebraic', '   ')

    simulation.run()

    # Retrieve the number of data points and output some or all of the
    # simulation values

    results = simulation.results()
    states = results.states()

    print('    - Result values:')
    print('       - Number of points: %d' % len(states['main/x'].values()))

    values(results.constants(), 'Constants')
    values(states, 'States')
    values(results.rates(), 'Rates')
    values(results.algebraic(), 'Algebraic')


def test_simulation(title, file_name_or_url, first=True):
    # Header

    header(title, first)

    # Open the simulation

    print(' - Open simulation')

    simulation = open_simulation(file_name_or_url)

    # Check whether the simulation is valid and has issues, and list some
    # information about it

    print(' - Check simulation:')
    print('    - Valid: %s' % ('yes' if simulation.valid() else 'no'))

    issues = simulation.issues()

    if issues:
        print('    - Issues:\n       - %s' % '\n       - '.join(issues))
    else:
        print('    - Issues: none')

    # Run #1: run the simulation using the default settings, except if we are
    #         dealing with a CellML file, in which case we set a few initial
    #         settings

    if file_name_or_url.endswith('.cellml'):
        data = simulation.data()

        data.set_ending_point(50.0)
        data.set_point_interval(0.001)

        print(' - Initial settings:')
        print('    - Ending point: %f' % data.ending_point())
        print('    - Point interval: %f' % data.point_interval())

    run_simulation(simulation, 1)

    # Run #2: change a few settings and rerun the simulation

    simulation.reset()
    simulation.clear_results()

    data = simulation.data()

    data.set_starting_point(10.0)
    data.set_ending_point(30.0)
    data.set_point_interval(0.1)
    data.set_ode_solver('Euler (forward)')
    data.set_ode_solver_property('Step', 0.01)

    print(' - New settings:')
    print('    - Starting point: %f' % data.starting_point())
    print('    - Ending point: %f' % data.ending_point())
    print('    - Point interval: %f' % data.point_interval())
    print('    - ODE solver: %s' % data.ode_solver_name())

    run_simulation(simulation, 2)

    # Run #3: carry on from the previous run

    run_simulation(simulation, 3)

    # Close the simulation

    oc.close_simulation(simulation)


if __name__ == '__main__':
    # Test for a local/remote CellML file

    test_simulation('Local CellML file', 'tests/cellml/lorenz.cellml')
    test_simulation('Remote CellML file',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml', False)

    # Test for a local/remote SED-ML file

    test_simulation('Local SED-ML file', 'tests/sedml/lorenz.sedml', False)
    test_simulation('Remote SED-ML file',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml', False)

    # Test for a local/remote COMBINE archive

    test_simulation('Local COMBINE archive', 'tests/combine/lorenz.omex', False)
    test_simulation('Remote COMBINE archive',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex', False)
