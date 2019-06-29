#!/bin/sh -x
QT_PATH=$HOME/Qt5.6.3/5.6.3/clang_64
RELEASE_VERSION=$(cat "release_version.txt")
echo $RELEASE_VERSION
SOURCE_PATH=$PWD
BUILD_NAME=die_mac_portable
GUIEXE=die
CONEXE=diec

cd $SOURCE_PATH/die_source

rm -rf $SOURCE_PATH/build

function makeproject
{
    cd $SOURCE_PATH/die_source/$1
    
    $QT_PATH/bin/qmake $1.pro -spec macx-clang CONFIG+=x86_64
    make -f Makefile clean
    make -f Makefile

    rm -rf Makefile
    rm -rf Makefile.Release
    rm -rf Makefile.Debug
    rm -rf object_script.*     

    cd $SOURCE_PATH
}

makeproject gui_source
makeproject console_source

mkdir -p $SOURCE_PATH/release
rm -rf $SOURCE_PATH/release/$BUILD_NAME
mkdir -p $SOURCE_PATH/release/$BUILD_NAME

cp -R $SOURCE_PATH/die_source/build/release/$GUIEXE.app               $SOURCE_PATH/release/$BUILD_NAME
cp -R $SOURCE_PATH/die_source/build/release/$CONEXE                  $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/MacOS/
mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/PlugIns

function fixlibrary
{
    install_name_tool -change @rpath/$1.framework/Versions/5/$1 @executable_path/../Frameworks/$1.framework/Versions/5/$1  $2    
}

function fiximport
{
    fixlibrary QtWidgets $1
    fixlibrary QtGui $1
    fixlibrary QtCore $1  
    #fixlibrary QtDBus $1
    fixlibrary QtPrintSupport $1
    fixlibrary QtScript $1
    fixlibrary QtSvg $1
    fixlibrary QtXml $1
    fixlibrary QtNetwork $1
    fixlibrary QtScriptTools $1
    fixlibrary QtConcurrent $1
}

function copylibrary
{
    mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks
    mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework
    mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework/Versions
    mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework/Versions/5
    
    cp -R $QT_PATH/lib/$1.framework/Versions/5/$1 $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework/Versions/5
    
    install_name_tool -id @executable_path/../Frameworks/$1.framework/Versions/5/$1 $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework/Versions/5/$1
    fiximport $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/Frameworks/$1.framework/Versions/5/$1
}

function copyplugin
{
    mkdir $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/PlugIns/$1/
    cp -R $QT_PATH/plugins/$1/$2.dylib $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/PlugIns/$1/
    
    install_name_tool -id @executable_path/../PlugIns/$1/$2.dylib $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/PlugIns/$1/$2.dylib
    fiximport $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/PlugIns/$1/$2.dylib
}

fiximport $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/MacOS/$GUIEXE
fiximport $SOURCE_PATH/release/$BUILD_NAME/$GUIEXE.app/Contents/MacOS/$CONEXE

copylibrary QtSvg
copylibrary QtWidgets
copylibrary QtGui
copylibrary QtCore
copylibrary QtPrintSupport
copylibrary QtXml
copylibrary QtScript
copylibrary QtNetwork
copylibrary QtScriptTools
copylibrary QtConcurrent

copyplugin platforms libqcocoa
copyplugin platforms libqminimal
copyplugin platforms libqoffscreen

copyplugin imageformats libqgif
copyplugin imageformats libqico
copyplugin imageformats libqjpeg
copyplugin imageformats libqmng
copyplugin imageformats libqsvg
copyplugin imageformats libqtga
copyplugin imageformats libqtiff

cp -R $SOURCE_PATH/Detect-It-Easy/db            $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/info          $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/scripts       $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/qss           $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/lang          $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/editor        $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/search        $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/
cp -R $SOURCE_PATH/Detect-It-Easy/yara          $SOURCE_PATH/release/${BUILD_NAME}/die.app/Contents/MacOS/

rm -rf $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.dmg
hdiutil create -format UDBZ -quiet -srcfolder $SOURCE_PATH/release/$BUILD_NAME $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.dmg
cd $SOURCE_PATH/release/
zip -r $SOURCE_PATH/release/${BUILD_NAME}_${RELEASE_VERSION}.zip ${BUILD_NAME}

rm -rf $SOURCE_PATH/release/$BUILD_NAME
