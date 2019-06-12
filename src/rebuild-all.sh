#!/bin/bash
set -eu

for d in dfu GameOfLife full stm32f070_miniblink BitbangIOTests; do
    make -C $d clean
    make -C $d all -j4
done
