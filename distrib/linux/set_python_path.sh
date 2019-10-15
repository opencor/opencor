#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Make sure the path to Python is correct

${OPENCOR_DIR}/python/bin/python ${OPENCOR_DIR}/python/bin/set_python_path.py ${OPENCOR_DIR}/python -s
