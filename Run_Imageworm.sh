#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source $SCRIPTPATH/env_setup.sh;

python $SCRIPTPATH/worm/worm_3.py;
