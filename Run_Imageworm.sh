#!/bin/bash
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
source $SCRIPTPATH/env_setup.sh;

python $SCRIPTPATH/worm/worm_3.py;
