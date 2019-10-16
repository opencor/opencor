#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

appDir="$(cd "$(dirname "$0")"; pwd)"

# Start Jupyter Lab

${appDir}/runjupyter.sh lab --ip 127.0.0.1 $*
