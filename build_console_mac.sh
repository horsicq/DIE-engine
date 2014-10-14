#!/bin/sh -x

qmake DIE_source/console/console.pro -r -spec macx-g++ CONFIG+=release CONFIG+=x86_64
make clean
make

rm -f Makefile
find . -type f -name "*.o" -exec rm -f {} \;
find . -type f -name "moc_*" -exec rm -f {} \;



