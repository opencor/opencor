if __name__ == '__main__':
    # Make sure that we can import the modules that should be bundled with our
    # Python package

    try:
        import bz2
    except ImportError:
        print("The 'bz2' module is not present!")

    try:
        import ipykernel
    except ImportError:
        print("The 'ipykernel' module is not present!")

    try:
        import IPython
    except ImportError:
        print("The 'IPython' module is not present!")

    try:
        import ipywidgets
    except ImportError:
        print("The 'ipywidgets' module is not present!")

    try:
        import jupyter_console
    except ImportError:
        print("The 'jupyter_console' module is not present!")

    try:
        import jupyterlab
    except ImportError:
        print("The 'jupyterlab' module is not present!")

    try:
        import lxml
    except ImportError:
        print("The 'lxml' module is not present!")

    try:
        import lzma
    except ImportError:
        print("The 'lzma' module is not present!")

    try:
        import matplotlib
    except ImportError:
        print("The 'matplotlib' module is not present!")

    try:
        import nbconvert
    except ImportError:
        print("The 'nbconvert' module is not present!")

    try:
        import notebook
    except ImportError:
        print("The 'notebook' module is not present!")

    try:
        import numpy
    except ImportError:
        print("The 'numpy' module is not present!")

    try:
        import qimage2ndarray
    except ImportError:
        print("The 'qimage2ndarray' module is not present!")

    try:
        import qtconsole
    except ImportError:
        print("The 'qtconsole' module is not present!")

    try:
        import scipy
    except ImportError:
        print("The 'scipy' module is not present!")

    try:
        import sphinx
    except ImportError:
        print("The 'sphinx' module is not present!")

    try:
        import sqlite3
    except ImportError:
        print("The 'sqlite3' module is not present!")

    try:
        import ssl
    except ImportError:
        print("The 'ssl' module is not present!")

    try:
        import traitlets
    except ImportError:
        print("The 'traitlets' module is not present!")

    try:
        import zlib
    except ImportError:
        print("The 'zlib' module is not present!")
