#!/bin/sh

appDir="$(cd "$(dirname "$0")"; pwd)"

${appDir}/runjupyter.sh notebook --ip 127.0.0.1 $*
