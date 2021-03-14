#!/bin/bash
set -e

pushd ../release/bin

export LD_LIBRARY_PATH=../lib
./tanks

popd
