#!/bin/sh

appDir="$(cd "$(dirname "$0")"; pwd)"

${appDir}/runjupyter.sh console --kernel OpenCOR $*
