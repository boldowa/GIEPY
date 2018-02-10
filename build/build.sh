#!/bin/sh
########################################
# build script
########################################

DBG=""
cd `dirname "$0"`

if test ! -z "$1"; then
	DBG="-DCMAKE_BUILD_TYPE=Debug"
fi

if test -r Makefile ; then
	echo "Cleaning..."
	make clean
fi

which cmake
if test ! $? ; then
	echo "Error: cmake isn't installed. pls install it."
	exit
fi

if test ! -e libasar.so -a ! -e ../libasar.so -a ! -e components/libasar.so ; then
	echo "Error: \"libasar.so\" is minnsing..."
	exit
fi

rm -rf CMake* cmake_* bolib compiler.h Makefile

echo "Build start..."
cmake $DBG -DJANSSON_WITHOUT_TESTS=ON -DJANSSON_BUILD_SHARED_LIBS=ON -DJANSSON_INSTALL=OFF -DJANSSON_BUILD_DOCS=OFF ..; make -j2
sync

