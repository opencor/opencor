import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *
from utils import *

if __name__ == '__main__':
    # Test the N62 model using different solvers

    run_simulations('noble_model_1962.cellml', 'N62 model')
