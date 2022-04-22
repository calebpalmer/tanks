BUILDDIR=build/caleb-desktop/bin/

#export ASAN_OPTIONS=detect_leaks=0

pushd $BUILDDIR
gdb --args tanks
popd
