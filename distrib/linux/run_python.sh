#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Make sure the path to Python is correct before starting Jupyter

${OPENCOR_DIR}/set_python_path.sh

# Start our script that runs Python code in OpenCOR

${OPENCOR_DIR}/python/bin/run_python.py $*
