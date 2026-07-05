#!/usr/bin/env bash
set -euo pipefail

X_SOURCE_PATH=$PWD
X_BUILD_NAME=die_mac_portable
X_RELEASE_VERSION=$(cat "release_version.txt")

BUILD_DIR=$(mktemp -d)
RELEASE_DIR="$X_SOURCE_PATH/release"

cleanup() { rm -rf "$BUILD_DIR"; }
trap cleanup EXIT

mkdir -p "$RELEASE_DIR"

QT_PREFIX_PATH="${1:-${QT_PREFIX_PATH:-$HOME/Qt/5.15.2/clang_64}}"
CMAKE_ARGS=(
    -S "$X_SOURCE_PATH"
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_PREFIX_PATH="$QT_PREFIX_PATH"
    -DCMAKE_OSX_ARCHITECTURES="${CMAKE_OSX_ARCHITECTURES:-x86_64}"
)

echo "Configuring..."
cmake "${CMAKE_ARGS[@]}"

echo "Building..."
cmake --build "$BUILD_DIR" --config Release -- -j$(sysctl -n hw.logicalcpu)

APP_BUNDLE=""
for candidate in \
    "$BUILD_DIR/src/gui/die.app" \
    "$BUILD_DIR/src/gui/DetectItEasy.app" \
    "$BUILD_DIR/src/gui/Release/die.app"
do
    [ -d "$candidate" ] && APP_BUNDLE="$candidate" && break
done

if [ -z "$APP_BUNDLE" ]; then
    echo "ERROR: app bundle not found under $BUILD_DIR" >&2
    exit 1
fi

RESOURCES="$APP_BUNDLE/Contents/Resources"
mkdir -p "$RESOURCES/signatures"

cp -Rf "$X_SOURCE_PATH/XStyles/qss"             "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/XInfoDB/info"            "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/Detect-It-Easy/db"       "$RESOURCES/"
[ -d "$X_SOURCE_PATH/Detect-It-Easy/db_extra" ] && cp -Rf "$X_SOURCE_PATH/Detect-It-Easy/db_extra" "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/XYara/yara_rules"        "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/images"                  "$RESOURCES/"
[ -f "$X_SOURCE_PATH/signatures/crypto.db" ]   && cp -f "$X_SOURCE_PATH/signatures/crypto.db" "$RESOURCES/signatures/"

# Copy diec into the bundle
DIEC_BIN=$(find "$BUILD_DIR/src/console" -maxdepth 2 -name "diec" -type f | head -1)
[ -n "$DIEC_BIN" ] && cp -f "$DIEC_BIN" "$APP_BUNDLE/Contents/MacOS/"

# macdeployqt (handled by cmake POST_BUILD, but run again as safety net)
MACDEPLOYQT=""
for candidate in \
    "$QT_PREFIX_PATH/bin/macdeployqt" \
    "$QT_PREFIX_PATH/../bin/macdeployqt"
do
    [ -x "$candidate" ] && MACDEPLOYQT="$candidate" && break
done

if [ -n "$MACDEPLOYQT" ]; then
    "$MACDEPLOYQT" "$APP_BUNDLE" -always-overwrite
fi

PACKAGE_DIR="$RELEASE_DIR/$X_BUILD_NAME"
rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR"
cp -R "$APP_BUNDLE" "$PACKAGE_DIR/"

cd "$RELEASE_DIR"
ZIP_NAME="die_${X_RELEASE_VERSION}_mac_portable.zip"
zip -r "$ZIP_NAME" "$X_BUILD_NAME"
rm -rf "$PACKAGE_DIR"
echo "Created: $RELEASE_DIR/$ZIP_NAME"
