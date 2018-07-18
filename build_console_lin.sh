#/bin/bash -x

QMAKE=/home/hors/QT/4.8.7/bin/qmake

$QMAKE DIE_source/console/console.pro -r -spec linux-g++
make clean
make

rm -f Makefile
find . -type f -name "*.o" -exec rm -f {} \;
find . -type f -name "moc_*" -exec rm -f {} \;



