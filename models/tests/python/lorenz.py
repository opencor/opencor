# Open our Lorenz SED-ML document

import os
import OpenCOR as oc

s = oc.openSimulation(os.path.dirname(__file__) + '/../sedml/lorenz/lorenz.sedml')

print('---------------------------------------')
print('Simulation:')
print(s)

# Run our Lorenz simulation

s.run()

# Retrieve our simulation results

r = s.results()

print('---------------------------------------')
print('Results:')
print(r)

# Retrieve our states

states = r.states()

print('---------------------------------------')
print('States:')
print(states)

# Retrieve the values for our states

x = states['main/x'].values()
y = states['main/y'].values()
z = states['main/z'].values()

print('---------------------------------------')
print('x, y and z:')
print(x, y, z)

# Generate a 3D plot for our states

from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

plt.axes(projection='3d').plot3D(x, y, z)
