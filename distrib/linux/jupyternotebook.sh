#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

appDir="$(cd "$(dirname "$0")"; pwd)"

# Start Jupyter notebook

${appDir}/runjupyter.sh notebook --ip 127.0.0.1 $*
