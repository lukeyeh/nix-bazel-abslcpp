let
  pkgs = import (fetchTarball ("channel:nixpkgs-unstable")) { };
in
pkgs.mkShell {
  buildInputs = [
    pkgs.clang
    pkgs.clang-tools
    pkgs.bazel
  ];
}
