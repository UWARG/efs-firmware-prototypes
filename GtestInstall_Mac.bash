#!/bin/bash

mkdir GtestTemp
cd GtestTemp

curl -LO https://github.com/google/googletest/archive/release-1.10.0.tar.gz
tar xf release-1.10.0.tar.gz
cd googletest-release-1.10.0
mkdir build
cd build
cmake ..
make
make install

cd ../../..
rm -r GtestTemp
