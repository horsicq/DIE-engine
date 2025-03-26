#!/bin/bash -x
mkdir release
cp -f PKGBUILD release/
cd release
makepkg -Acs OPTIONS=-debug 
rm -Rf PKGBUILD
rm -Rf DIE-engine
cd ..
