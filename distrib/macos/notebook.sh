#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Start Jupyter notebook

${OPENCOR_DIR}/jupyter.sh notebook $*
