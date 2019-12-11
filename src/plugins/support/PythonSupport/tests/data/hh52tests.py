import opencor as oc
import sys

sys.dont_write_bytecode = True

from basictests import *
from utils import *

if __name__ == '__main__':
    # Test the HH52 model using different solvers

    run_simulations('hodgkin_huxley_squid_axon_model_1952.cellml', 'HH52 model')
