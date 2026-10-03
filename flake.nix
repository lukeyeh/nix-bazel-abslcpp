{
  description = "Nix + Bazel + Abseil C++ template";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "aarch64-darwin" ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f nixpkgs.legacyPackages.${system});
    in
    {
      devShells = forAllSystems (pkgs:
        let
          # The compiler Bazel builds with. Bazel picks up $CC from this shell,
          # so the exact clang version is whatever flake.lock pins for this
          # LLVM major version.
          llvm = pkgs.llvmPackages_21;
        in
        {
          default = (pkgs.mkShell.override { stdenv = llvm.stdenv; }) {
            packages = [
              pkgs.bazel_9
              pkgs.bazel-buildtools
              llvm.clang-tools
            ];
          };
        });
    };
}
