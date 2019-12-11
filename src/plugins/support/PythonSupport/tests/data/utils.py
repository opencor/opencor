import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *


def run_simulation(simulation, solver_name):
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

    run_simulation(simulation, 'CVODE')
    run_simulation(simulation, 'Euler (forward)')
    run_simulation(simulation, 'Heun')
    run_simulation(simulation, 'Runge-Kutta (2nd order)')
    run_simulation(simulation, 'Runge-Kutta (4th order)')

    oc.close_simulation(simulation)
