#!/bin/bash
set -e

pushd ../build/bin

export LD_LIBRARY_PATH=../lib
gdb --args ./tanks

popd
