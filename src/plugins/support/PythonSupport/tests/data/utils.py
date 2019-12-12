import math
import opencor as oc
import os
import sys


def header(title, first=True):
    if not first:
        print()

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')


def open_simulation(file_name_or_url):
    if file_name_or_url.startswith('https://'):
        return oc.open_simulation(file_name_or_url)

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

        print('[ %f, %f, %f, ..., %f, %f, %f ]'
              % (data[0], data[1], data[2], data[data_len - 3], data[data_len - 2], data[data_len - 1]))


def values(data, data_type, indent=''):
    if data:
        print('%s    - %s:' % (indent, data_type))

        for item in data.values():
            print('%s       - %s = ' % (indent, item.uri()), end='')

            try:
                print_values(item.values())
            except Exception:
                print(item.value())
    else:
        print('%s    - %s: empty' % (indent, data_type))


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

    values(data.constants(), 'Constants', '      ')
    values(data.states(), 'States', '      ')
    values(data.rates(), 'Rates', '      ')
    values(data.algebraic(), 'Algebraic', '      ')

    simulation.run()

    # Retrieve the number of data points and output some or all of the
    # simulation values

    results = simulation.results()
    states = results.states()

    print('    - Result values:')
    print('       - Number of points: %d' % len(states['main/x'].values()))

    values(results.constants(), 'Constants', '   ')
    values(states, 'States', '   ')
    values(results.rates(), 'Rates', '   ')
    values(results.algebraic(), 'Algebraic', '   ')


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


def run_solver_simulation(simulation, solver_name):
    data = simulation.data()

    data.set_ode_solver(solver_name)
    data.set_ode_solver_property('Step', 0.01)

    simulation.reset()
    simulation.clear_results()
    simulation.run()

    results = simulation.results()

    print(' - %s:' % solver_name)

    values(results.constants(), 'Constants')
    values(results.states(), 'States')
    values(results.rates(), 'Rates')
    values(results.algebraic(), 'Algebraic')


def run_simulations(model, title):
    header(title)

    simulation = open_simulation(model)

    run_solver_simulation(simulation, 'CVODE')
    run_solver_simulation(simulation, 'Euler (forward)')
    run_solver_simulation(simulation, 'Heun')
    run_solver_simulation(simulation, 'Runge-Kutta (2nd order)')
    run_solver_simulation(simulation, 'Runge-Kutta (4th order)')

    oc.close_simulation(simulation)
