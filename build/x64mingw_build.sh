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

rm -rf CMake* cmake_* bolib compiler.h Makefile

echo "Build start..."
#cmake -DUNICODE=ON -DJANSSON_WITHOUT_TESTS=ON -DJANSSON_BUILD_SHARED_LIBS=ON -DJANSSON_INSTALL=OFF -DJANSSON_BUILD_DOCS=OFF -DCMAKE_TOOLCHAIN_FILE=../toolchain/mingw-x86.cmake ..; make -j2
cmake $DBG -DJANSSON_WITHOUT_TESTS=ON -DJANSSON_BUILD_SHARED_LIBS=ON -DJANSSON_INSTALL=OFF -DJANSSON_BUILD_DOCS=OFF -DCMAKE_TOOLCHAIN_FILE=../toolchain/mingw-x64.cmake ..; make -j2
sync

