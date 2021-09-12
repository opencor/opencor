import opencor as oc
import sys

sys.dont_write_bytecode = True

import utils

if __name__ == '__main__':
    # Test for a local/remote CellML file

    utils.test_simulation('Local CellML file', 'tests/cellml/lorenz.cellml')
    # utils.test_simulation('Remote CellML file',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml',
    #                       False)

    # Test for a local/remote SED-ML file

    utils.test_simulation('Local SED-ML file', 'tests/sedml/lorenz.sedml', False)
    # utils.test_simulation('Remote SED-ML file',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml',
    #                       False)

    # Test for a local/remote COMBINE archive

    utils.test_simulation('Local COMBINE archive', 'tests/combine/lorenz.omex', False)
    # utils.test_simulation('Remote COMBINE archive',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex',
    #                       False)

    # Test for a local/remote file that is not a CellML file, a SED-ML file or
    # a COMBINE archive

    utils.test_simulation('Local unknown file', 'tests/jupyter/lorenz.ipynb', False, True)
    # utils.test_simulation('Remote unknown file',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/jupyter/lorenz.ipynb',
    #                       False, True)

    # Test for a SED-ML file that uses an unsupported output start time

    utils.test_simulation('Unsupported local SED-ML file (output start time)',
                          'tests/sedml/lorenz_unsupported_output_start_time.sedml',
                          False, True)
    # utils.test_simulation('Unsupported remote SED-ML file (output start time)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz_unsupported_output_start_time.sedml',
    #                       False, True)

    # Test for a SED-ML file that uses an unsupported algorithm

    utils.test_simulation('Unsupported local SED-ML file (algorithm)',
                          'tests/sedml/lorenz_unsupported_algorithm.sedml',
                          False, True)
    # utils.test_simulation('Unsupported remote SED-ML file (algorithm)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz_unsupported_algorithm.sedml',
    #                       False, True)

    # Test for a SED-ML file that uses an unsupported algorithm

    utils.test_simulation('Unsupported local SED-ML file (algorithm parameter)',
                          'tests/sedml/lorenz_unsupported_algorithm_parameter.sedml',
                          False, True)
    # utils.test_simulation('Unsupported remote SED-ML file (algorithm parameter)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz_unsupported_algorithm_parameter.sedml',
    #                       False, True)

    # Test for a SED-ML file that uses an invalid parameter value

    utils.test_simulation('Invalid local SED-ML file (parameter value)',
                          'tests/sedml/lorenz_invalid_parameter_value.sedml',
                          False, True)
    # utils.test_simulation('Invalid remote SED-ML file (parameter value)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz_invalid_parameter_value.sedml',
    #                       False, True)

    # Test for a COMBINE archive that uses an unsupported output start time

    utils.test_simulation('Unsupported local COMBINE archive (output start time)',
                          'tests/combine/lorenz_unsupported_output_start_time.omex',
                          False, True)
    # utils.test_simulation('Unsupported remote COMBINE archive (output start time)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz_unsupported_output_start_time.omex',
    #                       False, True)

    # Test for a COMBINE archive that uses an unsupported algorithm

    utils.test_simulation('Unsupported local COMBINE archive (algorithm)',
                          'tests/combine/lorenz_unsupported_algorithm.omex',
                          False, True)
    # utils.test_simulation('Unsupported remote COMBINE archive (algorithm)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz_unsupported_algorithm.omex',
    #                       False, True)

    # Test for a COMBINE archive that uses an unsupported algorithm

    utils.test_simulation('Unsupported local COMBINE archive (algorithm parameter)',
                          'tests/combine/lorenz_unsupported_algorithm_parameter.omex',
                          False, True)
    # utils.test_simulation('Unsupported remote COMBINE archive (algorithm parameter)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz_unsupported_algorithm_parameter.omex',
    #                       False, True)

    # Test for a COMBINE archive that uses an invalid parameter value

    utils.test_simulation('Invalid local COMBINE archive (parameter value)',
                          'tests/combine/lorenz_invalid_parameter_value.omex',
                          False, True)
    # utils.test_simulation('Invalid remote COMBINE archive (parameter value)',
    #                       'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz_invalid_parameter_value.omex',
    #                       False, True)
