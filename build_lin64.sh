#/bin/bash -x

SOURCE_PATH=$PWD
QT_PATH=$HOME/Qt5.6.3/5.6.3/gcc_64
RELEASE_VERSION=$(cat "release_version.txt")
echo "Version:"
echo ${RELEASE_VERSION}

BUILD_NAME=die_lin64_portable
GUIEXE=die
CONEXE=diec

cd $SOURCE_PATH/die_source/gui_source

function makeproject
{
    cd $SOURCE_PATH/die_source/$1
    
    $QT_PATH/bin/qmake $1.pro -spec linux-g++
    make -f Makefile clean
    make -f Makefile

    rm -rf Makefile
    rm -rf Makefile.Release
    rm -rf Makefile.Debug
    rm -rf object_script.*     

    cd $SOURCE_PATH
}

rm -rf $SOURCE_PATH/build

makeproject console_source
makeproject lite_source
makeproject gui_source

mkdir -p $SOURCE_PATH/release
rm -rf $SOURCE_PATH/release/$BUILD_NAME
mkdir -p $SOURCE_PATH/release/$BUILD_NAME
mkdir -p $SOURCE_PATH/release/$BUILD_NAME/base
mkdir -p $SOURCE_PATH/release/$BUILD_NAME/base/platforms

cp -R $SOURCE_PATH/Detect-It-Easy/db            $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/info          $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/scripts       $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/qss           $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/lang          $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/editor        $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/search        $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/Detect-It-Easy/yara          $SOURCE_PATH/release/${BUILD_NAME}/base/

cp -R $SOURCE_PATH/die_source/build/release/die                $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/die_source/build/release/diec               $SOURCE_PATH/release/${BUILD_NAME}/base/
cp -R $SOURCE_PATH/die_source/build/release/diel               $SOURCE_PATH/release/${BUILD_NAME}/base/

cp -R $SOURCE_PATH/die_source/files/lin/die.sh                 $SOURCE_PATH/release/${BUILD_NAME}/
cp -R $SOURCE_PATH/die_source/files/lin/diec.sh                $SOURCE_PATH/release/${BUILD_NAME}/
cp -R $SOURCE_PATH/die_source/files/lin/diel.sh                $SOURCE_PATH/release/${BUILD_NAME}/

chmod +x $SOURCE_PATH/release/${BUILD_NAME}/die.sh
chmod +x $SOURCE_PATH/release/${BUILD_NAME}/diec.sh
chmod +x $SOURCE_PATH/release/${BUILD_NAME}/diel.sh

cp -R $QT_PATH/lib/libQt5Core.so.5.6.3                          $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Svg.so.5.6.3                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Gui.so.5.6.3                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Widgets.so.5.6.3                       $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5DBus.so.5.6.3                          $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Xml.so.5.6.3                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Script.so.5.6.3                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5XcbQpa.so.5.6.3                        $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libicui18n.so.56.1                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libicuuc.so.56.1                             $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libicudata.so.56.1                           $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Network.so.5.6.3                       $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5ScriptTools.so.5.6.3                   $SOURCE_PATH/release/$BUILD_NAME/base/
cp -R $QT_PATH/lib/libQt5Concurrent.so.5.6.3                    $SOURCE_PATH/release/$BUILD_NAME/base/

cp -R $QT_PATH/plugins/platforms/libqxcb.so                     $SOURCE_PATH/release/$BUILD_NAME/base/platforms/

mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Core.so.5.6.3                $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Core.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Svg.so.5.6.3                 $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Svg.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Gui.so.5.6.3                 $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Gui.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Widgets.so.5.6.3             $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Widgets.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5DBus.so.5.6.3                $SOURCE_PATH/release/$BUILD_NAME/base/libQt5DBus.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Xml.so.5.6.3                 $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Xml.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Script.so.5.6.3              $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Script.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5XcbQpa.so.5.6.3              $SOURCE_PATH/release/$BUILD_NAME/base/libQt5XcbQpa.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libicui18n.so.56.1                 $SOURCE_PATH/release/$BUILD_NAME/base/libicui18n.so.56
mv $SOURCE_PATH/release/$BUILD_NAME/base/libicuuc.so.56.1                   $SOURCE_PATH/release/$BUILD_NAME/base/libicuuc.so.56
mv $SOURCE_PATH/release/$BUILD_NAME/base/libicudata.so.56.1                 $SOURCE_PATH/release/$BUILD_NAME/base/libicudata.so.56
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Network.so.5.6.3             $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Network.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5ScriptTools.so.5.6.3         $SOURCE_PATH/release/$BUILD_NAME/base/libQt5ScriptTools.so.5
mv $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Concurrent.so.5.6.3          $SOURCE_PATH/release/$BUILD_NAME/base/libQt5Concurrent.so.5

chmod +x $SOURCE_PATH/release/$BUILD_NAME/die.sh
chmod +x $SOURCE_PATH/release/$BUILD_NAME/diec.sh
chmod +x $SOURCE_PATH/release/$BUILD_NAME/diel.sh

rm -r $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.tar
rm -r $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.tar.gz
cd $SOURCE_PATH/release/
tar -cvf $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.tar ${BUILD_NAME}
cd $SOURCE_PATH
gzip --best $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.tar

rm -rf $SOURCE_PATH/release/${BUILD_NAME}/
