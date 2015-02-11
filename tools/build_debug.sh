#!/usr/bin/env bash
set -e

pushd $(dirname $0) > /dev/null
SCRIPTPATH=$(pwd)
popd > /dev/null

mkdir $SCRIPTPATH"/../build"
cd $SCRIPTPATH"/../build"
cmake .. -DCMAKE_BUILD_TYPE=Debug
make VERBOSE=1 V=1
make install VERBOSE=1 V=1
ctest
