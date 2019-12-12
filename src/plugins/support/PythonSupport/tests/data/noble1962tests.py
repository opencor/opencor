import opencor as oc
import sys

sys.dont_write_bytecode = True

import utils

if __name__ == '__main__':
    # Test the Noble 1962 model using different solvers

    utils.run_simulations('noble_model_1962.cellml', 'Noble 1962 model')
