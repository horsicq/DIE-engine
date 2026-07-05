#!/bin/bash -e
export X_SOURCE_PATH=$PWD
export X_BUILD_NAME=die_linux_portable
export X_RELEASE_VERSION=$(cat "release_version.txt")

BUILD_DIR=$(mktemp -d)
STAGE_DIR=$(mktemp -d)
RELEASE_DIR="$X_SOURCE_PATH/release"

cleanup() { rm -rf "$BUILD_DIR" "$STAGE_DIR"; }
trap cleanup EXIT

mkdir -p "$RELEASE_DIR"
PACKAGE_NAME="die_${X_RELEASE_VERSION}_portable"
PACKAGE_DIR="$RELEASE_DIR/$PACKAGE_NAME"
rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR/base/platforms" "$PACKAGE_DIR/base/sqldrivers"

QT_PREFIX_PATH="${1:-}"
CMAKE_ARGS=(-S "$X_SOURCE_PATH" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release)
[ -n "$QT_PREFIX_PATH" ] && CMAKE_ARGS+=(-DCMAKE_PREFIX_PATH="$QT_PREFIX_PATH")

echo "Configuring..."
cmake "${CMAKE_ARGS[@]}"

echo "Building..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

cmake --install "$BUILD_DIR" --prefix "$STAGE_DIR"

cp -f "$STAGE_DIR/bin/die"   "$PACKAGE_DIR/base/"
cp -f "$STAGE_DIR/bin/diec"  "$PACKAGE_DIR/base/"
cp -f "$STAGE_DIR/bin/diel"  "$PACKAGE_DIR/base/"

if [ -n "$QT_PREFIX_PATH" ]; then
    X_QT_INSTALL_LIBS="$QT_PREFIX_PATH/lib"
    X_QT_INSTALL_PLUGINS="$QT_PREFIX_PATH/plugins"
    for lib in libQt5Core libQt5Gui libQt5Widgets libQt5Svg libQt5Sql libQt5Network \
               libQt5OpenGL libQt5DBus libQt5XcbQpa libQt5Script libQt5ScriptTools \
               libQt5Concurrent libQt5PrintSupport; do
        lib_file=$(find "$X_QT_INSTALL_LIBS" -maxdepth 1 -name "${lib}.so.5" 2>/dev/null | head -1)
        [ -n "$lib_file" ] && cp -Lf "$lib_file" "$PACKAGE_DIR/base/"
    done
    [ -f "$X_QT_INSTALL_PLUGINS/platforms/libqxcb.so" ] && \
        cp -f "$X_QT_INSTALL_PLUGINS/platforms/libqxcb.so" "$PACKAGE_DIR/base/platforms/"
    [ -f "$X_QT_INSTALL_PLUGINS/sqldrivers/libqsqlite.so" ] && \
        cp -f "$X_QT_INSTALL_PLUGINS/sqldrivers/libqsqlite.so" "$PACKAGE_DIR/base/sqldrivers/"
fi

# Copy data from cmake install staging (qss, db, info, yara, images, lang)
[ -d "$STAGE_DIR/lib/die/" ] && cp -Rf "$STAGE_DIR/lib/die/." "$PACKAGE_DIR/base/"
# Fallback: copy assets directly if cmake install didn't include them
[ ! -d "$PACKAGE_DIR/base/qss" ]        && cp -Rf "$X_SOURCE_PATH/XStyles/qss/"               "$PACKAGE_DIR/base/qss/"
[ ! -d "$PACKAGE_DIR/base/info" ]       && cp -Rf "$X_SOURCE_PATH/XInfoDB/info/"              "$PACKAGE_DIR/base/info/"
[ ! -d "$PACKAGE_DIR/base/db" ]         && cp -Rf "$X_SOURCE_PATH/Detect-It-Easy/db/"         "$PACKAGE_DIR/base/db/"
[ ! -d "$PACKAGE_DIR/base/yara_rules" ] && cp -Rf "$X_SOURCE_PATH/XYara/yara_rules/"          "$PACKAGE_DIR/base/yara_rules/"
[ ! -d "$PACKAGE_DIR/base/images" ]     && cp -Rf "$X_SOURCE_PATH/images/"                    "$PACKAGE_DIR/base/images/"
if [ -f "$X_SOURCE_PATH/signatures/crypto.db" ]; then
    mkdir -p "$PACKAGE_DIR/base/signatures"
    cp -f "$X_SOURCE_PATH/signatures/crypto.db" "$PACKAGE_DIR/base/signatures/"
fi

for bin in die diec diel; do
    cat > "$PACKAGE_DIR/${bin}.sh" <<EOF
#!/bin/sh
CWD=\$(dirname \$0)
export LD_LIBRARY_PATH="\$CWD/base:\$LD_LIBRARY_PATH"
"\$CWD/base/$bin" "\$@"
EOF
    chmod +x "$PACKAGE_DIR/${bin}.sh"
done

cd "$RELEASE_DIR"
tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME"
rm -rf "$PACKAGE_DIR"
echo "Created: $RELEASE_DIR/${PACKAGE_NAME}.tar.gz"
