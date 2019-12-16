import opencor as oc
import sys

sys.dont_write_bytecode = True

import utils

if __name__ == '__main__':
    # Test the Hodgkin–Huxley 1952 model using different solvers

    utils.run_simulations('hodgkin_huxley_squid_axon_model_1952.cellml', 'Hodgkin-Huxley 1952 model')
