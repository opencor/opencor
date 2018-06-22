Jupyter
=======

* Jupyter plugin needs instructions to add the kernel specification to an existing Jupyter installation.
* Need to add `OpenCOR` into PATH search so kernel can be started.
* Should Jupyter server be bundled?? No, but provide instructions as to how to do install and run using OpenCOR's Python.
* Multiple OpenCOR instances??
* An OpenCOR Jupyter kernel with no GUI:

    * requires a separation of the user interface from the running of a simulation in the simulation plugin.
    * result would be a new plugin that simply runs simulations.
    * this new plugin would simplify Jupyter kernel code and ease the development of a SED-ML CLI plugin.


Starting Jupyter Notebook
-------------------------

::

    ./jupyter.sh notebook --notebook-dir=NOTEBOOK_DIRECTORY


Debugging
---------

* Either use the file below or save a connection file by:
    1. Run `./jupyter.sh notebook --debug`
    2. Start a new OpenCOR notebook.
    3. Find the connection file details in the debug output and copy the file while the notebook is active.
    4. Shutdown Jupyter.

* Run OpenCOR under lldb, starting it with `run -c JupyterKernel::kernel CONNECTION_FILE`
* Run `jupyter qtconsole` and use `--existing CONNECTION_FILE` to attach to the OpenCOR kernel.
* Run Python code in the Qt console.


A Jupyter connection file: ::

    {
      "stdin_port": 65242,
      "iopub_port": 65241,
      "control_port": 65243,
      "signature_scheme": "hmac-sha256",
      "hb_port": 65244,
      "transport": "tcp",
      "shell_port": 65240,
      "key": "3dbb729c-a0cd99196a107d96884aa914",
      "kernel_name": "",
      "ip": "127.0.0.1"
    }
