#!/bin/bash -e
export X_SOURCE_PATH=$PWD
export X_RELEASE_VERSION=$(cat "release_version.txt")

BUILD_DIR=$(mktemp -d)
WORK_DIR=$(mktemp -d)
RELEASE_DIR="$X_SOURCE_PATH/release"

cleanup() { rm -rf "$BUILD_DIR" "$WORK_DIR"; }
trap cleanup EXIT

mkdir -p "$RELEASE_DIR"

QT_PREFIX_PATH="${1:-}"
CMAKE_ARGS=(-S "$X_SOURCE_PATH" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release)
[ -n "$QT_PREFIX_PATH" ] && CMAKE_ARGS+=(-DCMAKE_PREFIX_PATH="$QT_PREFIX_PATH")

echo "Configuring..."
cmake "${CMAKE_ARGS[@]}"

echo "Building..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

# Install into debian staging tree
STAGE_USR="$WORK_DIR/usr"
cmake --install "$BUILD_DIR" --prefix "$STAGE_USR"

# Ensure expected paths exist
mkdir -p "$WORK_DIR/usr/bin"
mkdir -p "$WORK_DIR/usr/lib/die/lang"
mkdir -p "$WORK_DIR/usr/share/applications"
mkdir -p "$WORK_DIR/usr/share/icons"
mkdir -p "$WORK_DIR/usr/share/metainfo"

# Copy assets not covered by cmake install (db, db_extra, peid, etc.)
[ -d "$X_SOURCE_PATH/Detect-It-Easy/db_extra" ] && \
    cp -Rf "$X_SOURCE_PATH/Detect-It-Easy/db_extra/." "$WORK_DIR/usr/lib/die/db_extra/"

# lang: cmake installs to prefix/lang; move to usr/lib/die/lang
if [ -d "$STAGE_USR/lang" ]; then
    cp -f "$STAGE_USR/lang/"*.qm "$WORK_DIR/usr/lib/die/lang/" 2>/dev/null || true
    rm -rf "$STAGE_USR/lang"
fi

# crypto.db
if [ -f "$X_SOURCE_PATH/signatures/crypto.db" ]; then
    mkdir -p "$WORK_DIR/usr/lib/die/signatures"
    cp -f "$X_SOURCE_PATH/signatures/crypto.db" "$WORK_DIR/usr/lib/die/signatures/"
fi

# Build DEBIAN/control
ARCH=$(dpkg --print-architecture 2>/dev/null || echo amd64)
mkdir -p "$WORK_DIR/DEBIAN"
cat > "$WORK_DIR/DEBIAN/control" <<EOF
Package: detectiteasy
Version: ${X_RELEASE_VERSION}
Architecture: ${ARCH}
Maintainer: hors <horsicq@gmail.com>
Homepage: http://ntinfo.biz
Section: devel
Priority: optional
Description: Detect It Easy is a program for determining types of files.
Depends: libqt5core5a, libqt5gui5, libqt5widgets5, libqt5svg5, libqt5sql5, libqt5network5, libqt5opengl5, libqt5dbus5
EOF

OS_VERSION=$(lsb_release -cs 2>/dev/null || echo linux)
DEB_NAME="die_${X_RELEASE_VERSION}_${OS_VERSION}_${ARCH}.deb"
dpkg-deb --build "$WORK_DIR" "$RELEASE_DIR/$DEB_NAME"
echo "Created: $RELEASE_DIR/$DEB_NAME"
