import os

import opencor as oc


def run_simulation(file_name_or_url):
    print(' - Open file...')

    if file_name_or_url.startswith('https://'):
        simulation = oc.openSimulation(file_name_or_url)
    else:
        simulation = oc.openSimulation(os.path.dirname(__file__) + '/' + file_name_or_url)

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

    oc.closeSimulation(simulation)


print('---------------------------------------')
print('           Local CellML file')
print('---------------------------------------')

run_simulation('../cellml/lorenz.cellml')

print()
print('---------------------------------------')
print('          Remote CellML file')
print('---------------------------------------')

run_simulation('https://raw.githubusercontent.com/agarny/opencor/issue1255/models/tests/cellml/lorenz.cellml')

print()
print('---------------------------------------')
print('           Local SED-ML file')
print('---------------------------------------')

run_simulation('../sedml/lorenz.sedml')

print()
print('---------------------------------------')
print('          Remote SED-ML file')
print('---------------------------------------')

run_simulation('https://raw.githubusercontent.com/agarny/opencor/issue1255/models/tests/sedml/lorenz.sedml')

print()
print('---------------------------------------')
print('         Local COMBINE archive')
print('---------------------------------------')

run_simulation('../combine/lorenz.omex')

print()
print('---------------------------------------')
print('        Remote COMBINE archive')
print('---------------------------------------')

run_simulation('https://raw.githubusercontent.com/agarny/opencor/issue1255/models/tests/combine/lorenz.omex')
