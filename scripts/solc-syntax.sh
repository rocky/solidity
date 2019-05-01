#!/usr/bin/env bash
for file in ../testp0/libsolidityp0/syntaxTests/parsing/errorRecovery/*.sol ; do
    echo $file ==============================
    ../build-phase0-debug/solc/solc $file
    echo $file ==============================
done
