import math
import opencor as oc
import os


def initial_values(data, type):
    if data:
        print('    - ' + type + ':')

        for item in data.values():
            print('       - %s = %f' % (item.uri(), item.value()))
    else:
        print('    - ' + type + ': N/A')


def run_simulation(simulation, step):
    # Run the simulation

    print(' - Run simulation... [%d]' % step)

    simulation.run()

    # Retrieve the simulation results and its corresponding data store, VOI,
    # constants, rates, states and algebraic information

    print('    - Retrieve simulation results...')

    results = simulation.results()
    data_store = results.data_store()
    voi = results.voi()
    constants = results.constants()
    rates = results.rates()
    states = results.states()
    algebraic = results.algebraic()

    print('      - VOI: ' + voi.name() + ' (' + voi.unit() + ')')

    print('    - Retrieve states...')

    states = results.states()

    print('    - Retrieve states\' values...')

    x = states['main/x'].values()
    y = states['main/y'].values()
    z = states['main/z'].values()


def test_simulation(title, file_name_or_url, first=True):
    if not first:
        print()

    # Header

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')

    # Open the simulation

    print(' - Open simulation...')

    if file_name_or_url.startswith('https://'):
        simulation = oc.open_simulation(file_name_or_url)
    else:
        simulation = oc.open_simulation(
            os.path.dirname(__file__) + '/../../../../../../models/tests/' + file_name_or_url)

    # Check whether the simulation is valid and has issues, and list some
    # information about it

    print(' - Check simulation...')
    print('    - Valid: ' + ('YES' if simulation.valid() else 'NO'))

    issues = simulation.issues()

    if issues:
        print('    - Issues:\n       - ' + '\n       - '.join(issues))
    else:
        print('    - Issues: N/A')

    data = simulation.data()

    print('    - Starting point: %f' % data.starting_point())
    print('    - Ending point:   %f' % data.ending_point())
    print('    - Point interval: %f' % data.point_interval())

    initial_values(data.constants(), "Constants")
    initial_values(data.states(), "States")
    initial_values(data.rates(), "Rates")
    initial_values(data.algebraic(), "Algebraic")

    # Run the simulation using the default settings

    run_simulation(simulation, 1)

    # Close the simulation

    oc.close_simulation(simulation)


# Test for an unknown local/remote file

try:
    test_simulation('Unknown local file', 'unknown')
except Exception as e:
    print(repr(e))

try:
    test_simulation('Unknown remote file',
                    'https://unknown', False)
except Exception as e:
    print(repr(e))

# Test for an invalid local/remote file

try:
    test_simulation('Invalid local file', 'cellml/underconstrained_model.cellml', False)
except Exception as e:
    print(repr(e))

try:
    test_simulation('Invalid remote file',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/underconstrained_model.cellml',
                    False)
except Exception as e:
    print(repr(e))

# Test for a local/remote CellML file

test_simulation('Local CellML file', 'cellml/lorenz.cellml', False)
test_simulation('Remote CellML file',
                'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml', False)

# Test for a local/remote SED-ML file

test_simulation('Local SED-ML file', 'sedml/lorenz.sedml', False)
test_simulation('Remote SED-ML file',
                'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml', False)

# Test for a local/remote COMBINE archive

test_simulation('Local COMBINE archive', 'combine/lorenz.omex', False)
test_simulation('Remote COMBINE archive',
                'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex', False)
