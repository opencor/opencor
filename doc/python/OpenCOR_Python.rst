Python in OpenCOR
=================

IPython console window
----------------------

A dockable window running `IPython <http://ipython.readthedocs.io/en/stable/index.html>`_ that includes `NumPy <http://www.numpy.org>`_, `Scipy library <https://scipy.org/scipylib/index.html>`_ and `matplotlib <http://matplotlib.org/>`_.

If not visible then enable it from OpenCOR's menu: ``View/Windows/PythonQtConsole``.

An IPython tutorial is `here <http://ipython.readthedocs.io/en/stable/interactive/tutorial.html>`_. Some keys are different than in the tutorial:

    ``Control-Enter``
        start multi-line input.
    ``Shift-Enter``
        execute code block.
    ``Escape``
        clear input.

Command history:

    Up/down keys
        History filtered with currently typed characters.
    %history
        All commands in current session.

Change font size in Python console:

    Linux and Windows
        ``Control +/-``
    MacOS
        ``Command +/-``

OpenCOR Python module
---------------------

.. code:: python

    import OpenCOR as oc

Methods:

    ``openSimulation()``
        Open a model. Corresponds to ``File/Open``.

        Returns the simulation.

    ``openRemoteSimulation()``
        Open a remote model. Corresponds to ``File/Open Remote``.

        Returns the simulation.

    ``simulation()``
        Returns the current simulation.

    ``simulations()``
        Returns a dictionary of open simulations.

Objects:

    ``Simulation``

    ``DataStore``

    ``DataStoreVariable``

Sample code
-----------

.. code:: python

    # Change the current directory
    %cd ~/biosignalml/workspace/cellml/workspaces/dbrooks/noble62

    # Import OpenCOR's Python module
    import OpenCOR as oc

    # Current (visible) simulation
    s = oc.simulation()

    # Dictionary of all open simulations
    s = oc.simulations()

    # Open a simulation (Name of CellML or SEDML file)
    s = oc.openSimulation('noble_1962.cellml')

    # Open a remote simulation (URL of a CellML or SEDML file)
    s = oc.openRemoteSimulation('https://models.physiomeproject.org/workspace/25d/rawfile/f0e63e292ebb1d6798da08914b4217aec434af96/lorenz.sedml')

    # Access a simulation's initial conditions
    d = s.data()

    # Time constraints
    d.startingPoint()
    d.endingPoint()
    d.pointInterval()

    # Changing values
    d.setStartingPoint(1)
    d.setEndingPoint(2)
    d.setPointInterval(0.1)

    # Parameters (as Python dictionaries)
    d.constants()
    d.states()
    d.rates()

    # Changing values
    d.constants()['main/k'] = 1.0

    # Run a simulation
    s.run()

    # Reset a simulation's parameters
    s.resetParameters()

    # Clear a simulation's results
    s.clearResults()

    # Access a simultaion's results
    r = s.results()

    # Time points
    t = r.points()
    t.values()

    # Dictionaries of DataStore variables
    r.constants()
    r.states()
    r.rates()
    r.algebraic()

    # Individual variables
    x = r.states()['main/k']
    x.values()

    # Data also accessible via the simulation's DataStore
    ds = r.dataStore()

    # Variable of integration (r.points())
    ds.voi()

    # Dictionaries of DataStore variables
    ds.variables()
    ds.voiAndVariables()

    # Individual variables
    x = ds.variables()['main/k']


matplotlib example
------------------

A matplotlib tutorial is `available here <https://matplotlib.org/users/pyplot_tutorial.html>`_.

.. code:: python

    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D

    # The following assumes a simulation of the Lorenz model has been run
    states = s.results().states()

    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.plot(states['main/x'].values(), states['main/y'].values(), states['main/z'].values(), linewidth=0.5)
    ax.set_xlabel(states['main/x'].label())
    ax.set_ylabel(states['main/y'].label())
    ax.set_zlabel(states['main/z'].label())
    plt.show()

..
    Other code: ::

    fig = plt.figure()

    ax = fig.add_subplot(2, 1, 1, projection='3d')
    ax.plot(t.values(), states['main/x'].values(), states['main/y'].values())
    ax.set_xlabel(t.label())
    ax.set_ylabel(states['main/x'].label())
    ax.set_zlabel(states['main/y'].label())

    ax = fig.add_subplot(2, 1, 2, projection='3d')
    ax.plot(states['main/x'].values(), states['main/y'].values(), states['main/z'].values())
    ax.set_ylabel(states['main/x'].label())
    ax.set_zlabel(states['main/y'].label())
    ax.set_zlabel(states['main/z'].label())

    plt.show()

