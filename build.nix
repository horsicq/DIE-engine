{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/refs/tags/24.05.tar.gz") {} }:

pkgs.stdenv.mkDerivation {
  pname = "detect-it-easy";
  version = "4.0.0";

  src = ./.;

  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
    libsForQt5.wrapQtAppsHook
  ];

  buildInputs = with pkgs; [
    libsForQt5.qtbase
    libsForQt5.qtscript
    libsForQt5.qtsvg
    libsForQt5.qttools
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];
}
