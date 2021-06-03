#!/bin/bash -x
export X_SOURCE_PATH=$PWD
export X_RELEASE_VERSION=$(cat "release_version.txt")

source build_tools/linux.sh

create_image_app_dir die

cp -f $X_SOURCE_PATH/build/release/die                              $X_SOURCE_PATH/release/appDir/usr/bin/
cp -f $X_SOURCE_PATH/build/release/diec                             $X_SOURCE_PATH/release/appDir/usr/bin/
cp -f $X_SOURCE_PATH/DEBIAN/die.desktop                             $X_SOURCE_PATH/release/appDir/usr/share/applications/
sed -i "s/#VERSION#/1.0/"                                           $X_SOURCE_PATH/release/appDir/usr/share/applications/die.desktop
cp -Rf $X_SOURCE_PATH/DEBIAN/hicolor/                               $X_SOURCE_PATH/release/appDir/usr/share/icons/
cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/info/                          $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -f $X_SOURCE_PATH/gui_source/translation/*.qm                    $X_SOURCE_PATH/release/appDir/usr/lib/die/lang/
mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures
cp -f $X_SOURCE_PATH/signatures/crypto.db                           $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures/

cd $X_SOURCE_PATH/release

linuxdeployqt $X_SOURCE_PATH/release/appDir/usr/share/applications/die.desktop -appimage -always-overwrite
mv mv *.AppImage die_${X_RELEASE_VERSION}.AppImage

cd $X_SOURCE_PATH

rm -Rf $X_SOURCE_PATH/release/appDir
