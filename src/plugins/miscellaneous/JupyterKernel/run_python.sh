#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Start our script that runs Python code in OpenCOR

${OPENCOR_DIR}/OpenCOR.app/Contents/Frameworks/Python/bin/python ${OPENCOR_DIR}/run_python.py $*
