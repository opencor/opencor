#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

appDir="$(cd "$(dirname "$0")"; pwd)"

# Start an IPython console attached to an OpenCOR kernel

${appDir}/runjupyter.sh console --kernel OpenCOR $*
