#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

appDir="$(cd "$(dirname "$0")"; pwd)"

# Make sure the path to Python is correct

${appDir}/python/bin/python ${appDir}/python/bin/setpythonpath.py ${appDir}/python -s
