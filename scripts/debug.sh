#!/bin/bash
set -e

export LD_LIBRARY_PATH=../lib
gdb --args ./tanks

popd
