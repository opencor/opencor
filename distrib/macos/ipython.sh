#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

OPENCOR_DIR="$(cd "$(dirname "$0")"; pwd)"

# Start an IPython console attached to an OpenCOR kernel

${OPENCOR_DIR}/run_jupyter.sh console --kernel OpenCOR $*
