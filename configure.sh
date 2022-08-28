#!/bin/bash
set -e
set -x

BUILDDIR=$1
INSTALLDIR=$BUILDDIR/install
BUILDTYPE=$2
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p $BUILDDIR
pushd $BUILDDIR

# export CC=/usr/bin/gcc
# export CXX=/usr/bin/g++

conan install $SCRIPT_DIR --profile $SCRIPT_DIR/conanprofile.txt --build missing

cmake -S${SCRIPT_DIR} -B. \
      -GNinja \
      -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
      -DCMAKE_BUILD_TYPE=$BUILDTYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1

popd
