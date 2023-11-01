#!/bin/bash

# Build and install pass-gen binary in /usr/bin/local/

# Usage: build.sh

make clean
make
install -v pass-gen /usr/local/bin
