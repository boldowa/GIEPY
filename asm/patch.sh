#!/bin/sh

function patch() {
	echo "--- patch roms/$1 -> $2"
	cp "roms/$1" "$2"
	asar giepy.asm "$2"
}

function main() {
	if test -z "$1"; then
		echo "--- Non EXBytes..."
		patch orig.smc test.smc
	elif test $1 -eq 1; then
		echo "--- has exbytes..."
		patch orig_eb.smc test_eb.smc
	else
		echo "--- sa-1..."
		patch sa1.smc test_sa1.smc
	fi

	echo "--- Done."
}

main $*
