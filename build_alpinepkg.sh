#!/bin/bash
set -ex

ROOT="$(pwd)"

echo "Building Alpine package in: $ROOT"

mkdir -p "$ROOT/release"

# Create build user if missing
if ! id builduser >/dev/null 2>&1; then
    adduser -D builduser
fi

# Setup abuild directory
mkdir -p /home/builduser/.abuild
chown -R builduser:builduser /home/builduser/.abuild

# Generate key as build user
if [ ! -f /home/builduser/.abuild/abuild.conf ]; then
    su builduser -c "abuild-keygen -n"

    cp /home/builduser/.abuild/*.rsa.pub /etc/apk/keys/

    cat > /home/builduser/.abuild/abuild.conf <<EOF
PACKAGER_PRIVKEY="/home/builduser/.abuild/$(basename /home/builduser/.abuild/*.rsa)"
EOF

    chown builduser:builduser /home/builduser/.abuild/abuild.conf
fi

# Give build user ownership
chown -R builduser:builduser "$ROOT"

# Build APK as non-root
su builduser -c "
    cd '$ROOT'
    abuild -r
"

# Copy generated APK
find /home/builduser/packages \
    -name "*.apk" \
    -exec cp {} "$ROOT/release/" \;

rm -f "$ROOT/release"/*debug*.apk || true

echo "Build output:"
ls -lah "$ROOT/release"

if ! ls "$ROOT/release"/*.apk >/dev/null 2>&1; then
    echo "ERROR: APK package was not generated"
    exit 1
fi

echo "Alpine package build successful"