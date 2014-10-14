#!/bin/sh -x

QMAKE_OPTIONS=''
if [[ "$OSTYPE"=="darwin13" ]]; then
    DIE_SOURCE='/Volumes/DATA/prepare/qt/DIE_source'
    QMAKE_OPTIONS='-r -spec macx-g++ CONFIG+=release CONFIG+=x86_64'
fi
function makeproject
{
    cd $DIE_SOURCE/$1
    qmake $2.pro $QMAKE_OPTIONS
    make clean
    make
    cd $DIE_SOURCE
}
function makeclean
{
	cd $DIE_SOURCE/$1
	make clean
	cd $DIE_SOURCE
}

qmake DIE_source/console/console.pro -r -spec macx-g++ CONFIG+=release CONFIG+=x86_64
make clean
make

find . -type f -name "*.o" -exec rm -f {} \;
find . -type f -name "moc_*" -exec rm -f {} \;



