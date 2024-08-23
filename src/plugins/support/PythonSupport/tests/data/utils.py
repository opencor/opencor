from enum import Enum
import math
import os

import opencor as oc


class Mode(Enum):
    RELEASE_NOTHING = 0
    RELEASE_VALUES = 1
    RELEASE_ALL_VALUES = 2


def header(title, first=True):
    if not first:
        print()

    header_length = 69

    print(header_length * '-')
    print(' ' * math.floor((header_length - len(title)) / 2) + title)
    print(header_length * '-')


def open_simulation(file_name_or_url):
    if file_name_or_url.startswith('https://'):
        return oc.open_simulation(file_name_or_url)

    original_directory = os.getcwd()

    os.chdir(os.path.dirname(__file__) + '/../../../../../../models/')

    simulation = oc.open_simulation(file_name_or_url)

    os.chdir(original_directory)

    return simulation


def str_value(value):
    res = format(value, '.1f')

    if res == '-0.0':
        return '0.0'

    return res


def print_values(data):
    if data is None:
        print('None')
    else:
        data_len = len(data)

        print('[ %s, %s, %s, ..., %s, %s, %s ]'
              % (str_value(data[0]), str_value(data[1]), str_value(data[2]),
                 str_value(data[data_len - 3]), str_value(data[data_len - 2]), str_value(data[data_len - 1])))


def values(simulation, mode, data, data_type, indent=''):
    if data:
        print('%s    - %s:' % (indent, data_type))

        for item in data.values():
            print('%s       - %s = ' % (indent, item.uri()), end='')

            try:
                values = item.values()

                print_values(values)

                if mode == Mode.RELEASE_VALUES:
                    simulation.release_values(values)
            except Exception:
                print(str_value(item.value()))
    else:
        print('%s    - %s: empty' % (indent, data_type))


def run_simulation(simulation, mode, step):
    # Run the simulation

    print(' - Run simulation [%d]:' % step)
    print('    - Settings:')

    data = simulation.data()

    print('       - Starting point: %f' % data.starting_point())
    print('       - Ending point: %f' % data.ending_point())
    print('       - Point interval: %f' % data.point_interval())
    print('       - ODE solver: %s' % data.ode_solver_name())
    print('       - Initial values:')

    values(simulation, mode, data.constants(), 'Constants', '      ')
    values(simulation, mode, data.states(), 'States', '      ')
    values(simulation, mode, data.rates(), 'Rates', '      ')
    values(simulation, mode, data.algebraic(), 'Algebraic', '      ')

    simulation.run()

    # Retrieve the number of data points and output some or all of the
    # simulation values

    results = simulation.results()
    states = results.states()

    print('    - Result values:')
    print('       - Number of points: %d' % states['main/x'].values_count())

    values(simulation, mode, results.constants(), 'Constants', '   ')
    values(simulation, mode, states, 'States', '   ')
    values(simulation, mode, results.rates(), 'Rates', '   ')
    values(simulation, mode, results.algebraic(), 'Algebraic', '   ')

    if mode == Mode.RELEASE_ALL_VALUES:
        simulation.release_all_values()


def test_simulation(title, file_name_or_url, first=True, expected_fail=False):
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

    if expected_fail:
        return

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

    run_simulation(simulation, Mode.RELEASE_ALL_VALUES , 1)

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

    run_simulation(simulation, Mode.RELEASE_VALUES, 2)

    # Run #3: carry on from the previous run

    run_simulation(simulation, Mode.RELEASE_NOTHING, 3)

    # Close the simulation

    oc.close_simulation(simulation)


def run_solver_simulation(simulation, mode, solver_name):
    data = simulation.data()

    data.set_ode_solver(solver_name)
    data.set_ode_solver_property('Step', 0.01)

    simulation.reset()
    simulation.clear_results()
    simulation.run()

    results = simulation.results()

    print(' - %s:' % solver_name)

    values(simulation, mode, results.constants(), 'Constants')
    values(simulation, mode, results.states(), 'States')
    values(simulation, mode, results.rates(), 'Rates')
    values(simulation, mode, results.algebraic(), 'Algebraic')

    if mode == Mode.RELEASE_ALL_VALUES:
        simulation.release_all_values()


def run_simulations(model, title):
    header(title)

    simulation = open_simulation(model)

    run_solver_simulation(simulation, Mode.RELEASE_NOTHING, 'CVODE')
    run_solver_simulation(simulation, Mode.RELEASE_VALUES, 'Euler (forward)')
    run_solver_simulation(simulation, Mode.RELEASE_ALL_VALUES, 'Heun')
    run_solver_simulation(simulation, Mode.RELEASE_VALUES, 'Runge-Kutta (2nd order)')
    run_solver_simulation(simulation, Mode.RELEASE_NOTHING, 'Runge-Kutta (4th order)')

    oc.close_simulation(simulation)
