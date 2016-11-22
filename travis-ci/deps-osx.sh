#!/bin/bash

set -e

brew update
brew tap homebrew/science
brew tap haraldf/kf5
brew tap polakovic/astronomy

CELLAR_URL=https://s3-us-west-1.amazonaws.com/kstars-cache.cerebiggum.com/Cellar.tgz

echo "Downloading Cellar..."
mkdir -p /usr/local
curl -o /usr/local/Cellar.tgz ${CELLAR_URL}
cd /usr/local

echo "Extracting Cellar..."
tar xfz Cellar.tgz

echo "Linking Cellar..."
brew link