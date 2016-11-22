#!/bin/bash

echo "Finding dependency paths..."
export PATH=$PATH:$(brew --prefix gettext)/bin
export Qt5_DIR=$(brew --prefix qt5)
export Qt5DBus_DIR=$Qt5_DIR
export Qt5Test_DIR=$Qt5_DIR
export Qt5Network_DIR=$Qt5_DIR

echo "Creating build folder..."
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir -p ${DIR}/../build
cd ${DIR}/../build

echo "Generating makefiles..."
cmake ..
