#!/bin/bash

cmake -DCMAKE_BUILD_TYPE='Debug' -DCMAKE_INSTALL_PREFIX:PATH=$HOME/local -DBUILD_SHARED_LIBS=on -DLLVM_ROOT="/home/iwasz/local/share/llvm/" -DBOOST_ROOT="/home/iwasz/local" ..

