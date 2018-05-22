Jupyter
~~~~~~~

* Jupyter plugin needs instructions to add the kernel specification to an existing Jupyter installation.
* Need to add `OpenCOR` into PATH search so kernel can be started.
* Should Jupyter server be bundled?? No, but provide instructions as to how to do install and run using OpenCOR's Python.
* Multiple OpenCOR instances??
* An OpenCOR Jupyter kernel with no GUI:

    * requires a separation of the user interface from the running of a simulation in the simulation plugin.
    * result would be a new plugin that simply runs simulations.
    * this new plugin would simplify Jupyter kernel code and ease the development of a SED-ML CLI plugin.
