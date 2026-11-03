#!/usr/bin/env bash
set -euo pipefail

X_SOURCE_PATH=$PWD
X_BUILD_NAME=die_mac_portable
X_RELEASE_VERSION=$(cat "release_version.txt")

BUILD_DIR=$(mktemp -d)
RELEASE_DIR="$X_SOURCE_PATH/release"
ZIP_NAME="die_${X_RELEASE_VERSION}_mac_portable.zip"
DMG_NAME="die_${X_RELEASE_VERSION}_mac.dmg"
PACKAGE_DIR="$RELEASE_DIR/$X_BUILD_NAME"

cleanup() { rm -rf "$BUILD_DIR" "$PACKAGE_DIR"; }
trap cleanup EXIT

mkdir -p "$RELEASE_DIR"
# A failed build must not leave older packages looking like new output.
rm -f "$RELEASE_DIR/$ZIP_NAME" "$RELEASE_DIR/$DMG_NAME"

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

cp -Rf "$X_SOURCE_PATH/dep/XStyles/qss"             "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/dep/XInfoDB/info"            "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/dep/Detect-It-Easy/db"       "$RESOURCES/"
for extra_dir in "$X_SOURCE_PATH/dep/Detect-It-Easy/db_extra"/*/; do
    [ -d "$extra_dir" ] || continue
    cp -Rf "$extra_dir" "$RESOURCES/db/"
done
cp -Rf "$X_SOURCE_PATH/dep/XYara/yara_rules"        "$RESOURCES/"
cp -Rf "$X_SOURCE_PATH/images"                  "$RESOURCES/"
[ -f "$X_SOURCE_PATH/dep/signatures/crypto.db" ]   && cp -f "$X_SOURCE_PATH/dep/signatures/crypto.db" "$RESOURCES/signatures/"

# Copy diec into its final bundle location before the last deployment pass.
DIEC_BIN=$(find "$BUILD_DIR/src/console" -maxdepth 2 -name "diec" -type f -print -quit)
if [ -z "$DIEC_BIN" ]; then
    echo "ERROR: diec not found under $BUILD_DIR/src/console" >&2
    exit 1
fi

BUNDLED_DIEC="$APP_BUNDLE/Contents/MacOS/diec"
cp -f "$DIEC_BIN" "$BUNDLED_DIEC"

MACDEPLOYQT_EXECUTABLES=("-executable=$BUNDLED_DIEC")
# CMake can leave out the optional updater when its sources are unavailable.
if [ -d "$BUILD_DIR/src/updater" ]; then
    UPDATER_BIN=$(find "$BUILD_DIR/src/updater" -maxdepth 2 -name "die-update" -type f -print -quit)
    if [ -n "$UPDATER_BIN" ]; then
        BUNDLED_UPDATER="$APP_BUNDLE/Contents/MacOS/die-update"
        cp -f "$UPDATER_BIN" "$BUNDLED_UPDATER"
        MACDEPLOYQT_EXECUTABLES+=("-executable=$BUNDLED_UPDATER")
    fi
fi

# macdeployqt (handled by cmake POST_BUILD, but run again as safety net)
MACDEPLOYQT=""
for candidate in \
    "$QT_PREFIX_PATH/bin/macdeployqt" \
    "$QT_PREFIX_PATH/../bin/macdeployqt"
do
    [ -x "$candidate" ] && MACDEPLOYQT="$candidate" && break
done

if [ -z "$MACDEPLOYQT" ]; then
    echo "ERROR: macdeployqt not found under $QT_PREFIX_PATH" >&2
    exit 1
fi

# Both Qt 5 and Qt 6 require non-main executables to be listed explicitly.
"$MACDEPLOYQT" "$APP_BUNDLE" \
    "${MACDEPLOYQT_EXECUTABLES[@]}" \
    -always-overwrite

rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR"
cp -R "$APP_BUNDLE" "$PACKAGE_DIR/"
STAGED_APP="$PACKAGE_DIR/${APP_BUNDLE##*/}"

# Package exactly the deployed bundle used by the portable ZIP. The Applications
# shortcut supports drag-and-drop installation from the mounted disk image.
# Signing and notarization are performed separately by the release workflow.
DMG_DIR="$BUILD_DIR/dmg"
mkdir -p "$DMG_DIR"
chmod 755 "$DMG_DIR"
ditto "$STAGED_APP" "$DMG_DIR/${APP_BUNDLE##*/}"
ln -s /Applications "$DMG_DIR/Applications"
hdiutil create -volname "Detect It Easy" -srcfolder "$DMG_DIR" \
    -fs HFS+ -format UDZO -ov "$BUILD_DIR/$DMG_NAME"
if [ ! -s "$BUILD_DIR/$DMG_NAME" ]; then
    echo "ERROR: hdiutil did not create a non-empty disk image" >&2
    exit 1
fi
hdiutil verify "$BUILD_DIR/$DMG_NAME"

cd "$RELEASE_DIR"
zip -y -r "$BUILD_DIR/$ZIP_NAME" "$X_BUILD_NAME"
if [ ! -s "$BUILD_DIR/$ZIP_NAME" ]; then
    echo "ERROR: zip did not create a non-empty archive" >&2
    exit 1
fi
mv -f "$BUILD_DIR/$ZIP_NAME" "$RELEASE_DIR/$ZIP_NAME"
mv -f "$BUILD_DIR/$DMG_NAME" "$RELEASE_DIR/$DMG_NAME"
rm -rf "$PACKAGE_DIR"
echo "Created: $RELEASE_DIR/$ZIP_NAME"
echo "Created: $RELEASE_DIR/$DMG_NAME"
