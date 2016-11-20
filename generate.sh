#!/bin/bash

SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
BUILD_PATH=${SCRIPTPATH}/build

rm -rf ${BUILD_PATH}

mkdir ${BUILD_PATH}

cd ${BUILD_PATH}

#CXX=clang++ CC=clang cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ${SCRIPTPATH}
CXX=clang++ CC=clang cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" ${SCRIPTPATH}
