#!/bin/bash -x
export X_SOURCE_PATH=$PWD
export X_RELEASE_VERSION=$(cat "release_version.txt")
export VERSION=$X_RELEASE_VERSION

source dep/build_tools/linux.sh

create_image_app_dir die

cp -f $X_SOURCE_PATH/build/release/die $X_SOURCE_PATH/release/appDir/usr/bin/
mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/die
UPDATER_DEPLOY_ARGS=()
if [ -f "$X_SOURCE_PATH/build/release/die-update" ]; then
    cp -f "$X_SOURCE_PATH/build/release/die-update" "$X_SOURCE_PATH/release/appDir/usr/lib/die/" || exit $?
    ln -sf ../lib/die/die-update "$X_SOURCE_PATH/release/appDir/usr/bin/die-update" || exit $?
    UPDATER_DEPLOY_ARGS=(--executable "$X_SOURCE_PATH/release/appDir/usr/lib/die/die-update")
fi
#cp -f $X_SOURCE_PATH/build/release/diec $X_SOURCE_PATH/release/appDir/usr/bin/
#cp -f $X_SOURCE_PATH/build/release/diel $X_SOURCE_PATH/release/appDir/usr/bin/

cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.desktop $X_SOURCE_PATH/release/appDir/usr/share/applications/
cp -Rf $X_SOURCE_PATH/LINUX/hicolor/ $X_SOURCE_PATH/release/appDir/usr/share/icons/
cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.metainfo.xml $X_SOURCE_PATH/release/appDir/usr/share/metainfo/
cp -Rf $X_SOURCE_PATH/images/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/dep/XStyles/qss/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/dep/XInfoDB/info/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/dep/Detect-It-Easy/db/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/dep/Detect-It-Easy/db_custom/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
for db_extra_dir in "$X_SOURCE_PATH"/dep/Detect-It-Easy/db_extra/*/; do
    if [ -d "$db_extra_dir" ]; then
        cp -Rf "$db_extra_dir" "$X_SOURCE_PATH/release/appDir/usr/lib/die/db/" || exit $?
    fi
done
cp -Rf $X_SOURCE_PATH/dep/XYara/yara_rules/ $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/dep/XPEID/peid/ $X_SOURCE_PATH/release/appDir/usr/lib/die/

mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures
cp -f $X_SOURCE_PATH/dep/signatures/crypto.db $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures/

mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/platforms
cp /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/libqxcb.so $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/platforms/

mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/imageformats
cp /usr/lib/x86_64-linux-gnu/qt5/plugins/imageformats/libqjpeg.so $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/imageformats/

mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/printsupport
cp /usr/lib/x86_64-linux-gnu/qt5/plugins/printsupport/libcupsprintersupport.so $X_SOURCE_PATH/release/appDir/usr/lib/qt5/plugins/printsupport/

cd $X_SOURCE_PATH/release

linuxdeploy \
  --appdir $X_SOURCE_PATH/release/appDir \
  --plugin qt \
  --executable $X_SOURCE_PATH/release/appDir/usr/bin/die \
  "${UPDATER_DEPLOY_ARGS[@]}" \
  --output appimage

cd $X_SOURCE_PATH
rm -Rf $X_SOURCE_PATH/release/appDir
