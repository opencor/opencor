#!/bin/sh

crtDir="$(cd "$(dirname "$0")"; pwd)"

${crtDir}/scripts/genericmake Release $*
