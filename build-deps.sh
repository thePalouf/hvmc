#!/bin/bash

mkdir -p deps
cd deps
wget https://www.libsdl.org/release/SDL2-2.0.3.tar.gz
tar xvf SDL2-2.0.3.tar.gz
cd SDL2-2.0.3
./configure --prefix=$PWD
make -j4
make install
cd ../..
