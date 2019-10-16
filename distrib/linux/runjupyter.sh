#!/bin/sh

appDir="$(cd "$(dirname "$0")"; pwd)"

${appDir}/setpythonpath.sh

export PATH=${appDir}:$PATH

${appDir}/python/bin/jupyter $*
