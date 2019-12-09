import math
import opencor as oc
import os


def run_simulation(title: str, file_name_or_url: str, first: bool = True) -> object:
    if not first:
        print()

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')

    print(' - Open file...')

    if file_name_or_url.startswith('https://'):
        simulation = oc.open_simulation(file_name_or_url)
    else:
        simulation = oc.open_simulation(
            os.path.dirname(__file__) + '/../../../../../../models/tests/' + file_name_or_url)

    print(' - Check file...')
    print('    - Valid: ' + ('YES' if simulation.valid() else 'NO'))

    issues = simulation.issues()

    if issues:
        print('    - Issues:\n       - ' + '\n       - '.join(simulation.issues()))
    else:
        print('    - Issues: N/A')

    print(' - Run file...')

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


try:
    run_simulation('Unknown local file', 'unknown')
except Exception as e:
    print(repr(e))

try:
    run_simulation('Unknown remote file',
                   'https://unknown', False)
except Exception as e:
    print(repr(e))

run_simulation('Local CellML file', 'cellml/lorenz.cellml', False)
run_simulation('Remote CellML file',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml', False)

run_simulation('Local SED-ML file', 'sedml/lorenz.sedml', False)
run_simulation('Remote SED-ML file',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml', False)

run_simulation('Local COMBINE archive', 'combine/lorenz.omex', False)
run_simulation('Remote COMBINE archive',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex', False)
