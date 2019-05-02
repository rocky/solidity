#!/usr/bin/env bash
echo "${BASH_SOURCE[0]%/*}"
cd "${BASH_SOURCE[0]%/*}"
cd ..
ROOTDIR=$(pwd)
BUILDDIR="${ROOTDIR}/build-phase0-debug"
solc=${ROOTDIR}/build-phase0-debug/solc/solc
cd ${ROOTDIR}/testp0/libsolidityp0/syntaxTests/parsing/errorRecovery/
for file in *.sol ; do
    echo ==============================
    echo $(pwd)/$file
    $solc $file
    echo $file ==============================
done
