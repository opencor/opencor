import math
import os

import opencor as oc


def run_simulation(title, file_name_or_url, first=True):
    if not first:
        print()

    print('---------------------------------------')
    print(' ' * math.floor((39 - len(title)) / 2) + title)
    print('---------------------------------------')

    print(' - Open file...')

    if file_name_or_url.startswith('https://'):
        simulation = oc.open_simulation(file_name_or_url)
    else:
        simulation = oc.open_simulation(os.path.dirname(__file__) + '/' + file_name_or_url)

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


run_simulation('Local CellML file', '../cellml/lorenz.cellml')
run_simulation('Remote CellML file',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml', False)

run_simulation('Local SED-ML file', '../sedml/lorenz.sedml', False)
run_simulation('Remote SED-ML file',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml', False)

run_simulation('Local COMBINE archive', '../combine/lorenz.omex', False)
run_simulation('Remote COMBINE archive',
               'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex', False)
