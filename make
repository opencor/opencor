#!/bin/sh

homeDir="$(cd "$(dirname "$0")"; pwd)"

${homeDir}/scripts/genericmake Release $*
