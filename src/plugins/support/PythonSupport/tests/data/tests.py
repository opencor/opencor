import math
import opencor as oc
import os


def test_simulation(title: str, file_name_or_url: str, first: bool = True) -> None:
    if not first:
        print()

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')

    print(' - Open simulation...')

    if file_name_or_url.startswith('https://'):
        simulation = oc.open_simulation(file_name_or_url)
    else:
        simulation = oc.open_simulation(
            os.path.dirname(__file__) + '/../../../../../../models/tests/' + file_name_or_url)

    print(' - Check simulation...')
    print('    - Valid: ' + ('YES' if simulation.valid() else 'NO'))

    issues = simulation.issues()

    if issues:
        print('    - Issues:\n       - ' + '\n       - '.join(simulation.issues()))
    else:
        print('    - Issues: N/A')

    print(' - Run simulation...')

    simulation.run()

    print(' - Retrieve simulation results...')

    results = simulation.results()

    print(' - Retrieve states...')

    states = results.states()

    print(' - Retrieve states\' values...')

    x = states['main/x'].values()
    y = states['main/y'].values()
    z = states['main/z'].values()

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
