#!/bin/sh

# Get the script's directory, which should be OpenCOR's base directory

DIR=$(dirname "$0")

# Make sure the path to Python is correct before starting Jupyter

${DIR}/python/bin/python ${DIR}/python/bin/set_python_path.py ${DIR}/python -s

${DIR}/python/bin/jupyter $*
