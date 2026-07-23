pkgname=detect-it-easy
pkgver=4.0.0
pkgrel=0

pkgdesc="Detect It Easy (DIE) is a program for determining types of files"
url="https://github.com/horsicq/Detect-It-Easy"
maintainer="horsicq <horsicq@gmail.com>"

arch="x86_64"
license="MIT"

depends="
    qt5-qtbase
    qt5-qtscript
    qt5-qtsvg
"

makedepends="
    build-base
    ninja
    cmake
    qt5-qtbase-dev
    qt5-qtscript-dev
    qt5-qttools-dev
    qt5-qtsvg-dev
    pkgconf
"

options="!check"


build() {
    cmake \
        -B build \
        -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_C_FLAGS_RELEASE="-O3 -DNDEBUG" \
        -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG"

    cmake --build build -j$(nproc)
}


package() {
    DESTDIR="$pkgdir" \
        cmake --install build

    # remove debug files
    find "$pkgdir" -name "*.debug" -delete

    # strip binaries and shared libraries
    find "$pkgdir" \
        -type f \
        \( -executable -o -name "*.so*" \) \
        -exec strip --strip-unneeded {} + 2>/dev/null || true

    # install license
    install -Dm644 LICENSE \
        "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}