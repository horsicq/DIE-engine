#!/bin/bash
set -ex

ROOT="$(pwd)"

echo "Building Alpine package in: $ROOT"

# Create release directory
mkdir -p "$ROOT/release"

# Setup abuild key for CI build
mkdir -p "$HOME/.abuild"

if [ ! -f "$HOME/.abuild/abuild.conf" ]; then
    abuild-keygen -a -i -n
fi

# Build APK using APKBUILD from repository root
abuild -r

# Copy generated APK
find "$HOME/packages" \
    -name "*.apk" \
    -exec cp {} "$ROOT/release/" \;

# Remove debug packages
rm -f "$ROOT/release"/*debug*.apk || true

echo "Build output:"
ls -lah "$ROOT/release"

# Validate package exists
if ! ls "$ROOT/release"/*.apk >/dev/null 2>&1; then
    echo "ERROR: APK package was not generated"
    exit 1
fi

echo "Alpine package build successful"
