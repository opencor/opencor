import os

import OpenCOR as oc


def run_simulation(file_name):
    print(' - Open file...')

    simulation = oc.openSimulation(os.path.dirname(__file__) + file_name)

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

    return simulation


print('---------------------------------------')
print('              CellML file')
print('---------------------------------------')

cellml = run_simulation('/../cellml/lorenz.cellml')

print()
print('---------------------------------------')
print('              SED-ML file')
print('---------------------------------------')

sedml = run_simulation('/../sedml/lorenz.sedml')

print()
print('---------------------------------------')
print('            COMBINE archive')
print('---------------------------------------')

omex = run_simulation('/../combine/lorenz.omex')

print()
print('---------------------------------------')
print('               Shutdown')
print('---------------------------------------')

oc.closeSimulation(cellml)
oc.closeSimulation(sedml)
oc.closeSimulation(omex)
