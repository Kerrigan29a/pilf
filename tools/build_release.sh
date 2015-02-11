#!/usr/bin/env bash

pushd $(dirname $0) > /dev/null
SCRIPTPATH=$(pwd)
popd > /dev/null

mkdir $SCRIPTPATH"/../build"
cd $SCRIPTPATH"/../build"
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
