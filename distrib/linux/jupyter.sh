#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Make sure the path to Python is correct before starting Jupyter

${OPENCOR_DIR}/python/bin/python ${OPENCOR_DIR}/python/bin/set_python_path.py ${OPENCOR_DIR}/python -s

# Make sure we can find the OpenCOR application to run as a Jupyter kernel

export PATH=${OPENCOR_DIR}:$PATH

# Start Jupyter

${OPENCOR_DIR}/python/bin/jupyter $*
