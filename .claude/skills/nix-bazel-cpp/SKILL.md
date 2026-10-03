---
name: nix-bazel-cpp
description: Scaffold a new C++ project built with Bazel (Bzlmod) and Abseil, with a Nix flake dev shell, GoogleTest, Google Benchmark, and clangd compile_commands.json support. Use when the user wants to start a new C++/Bazel/Nix project, or asks for a project "like nix-bazel-abslcpp".
---

# Nix + Bazel + Abseil C++ project

Sets up a project modelled on https://github.com/lukeyeh/nix-bazel-abslcpp.
The files at the root of that repo are a known-good starting point; if this
skill is installed standalone, a copy lives in `template/` next to this file.

## Steps

1. **Create the repo with jj** (the user uses jj, not raw git):
   `jj git init <dir>` (colocated, so Nix flakes can see the files).
2. **Copy the template**: the repo's root files (not `.git`, `.jj`, `README.md`
   or `flake.lock`'s stale pins if you'll update anyway) into the project root.
   From a standalone `template/`, rename `gitignore` to `.gitignore`. Don't copy `MODULE.bazel.lock` from
   elsewhere; Bazel generates it.
3. **Rename**: set `module(name = ...)` in `MODULE.bazel` and the flake
   `description` to the project name. Replace the `greeting`/`hello_world`
   example with the user's code if they described what they're building;
   otherwise leave it as a working example.
4. **Refresh versions** (the template's pins go stale):
   - `nix flake update`, then set `.bazelversion` to what `bazel --version`
     prints inside `nix develop`. Nix's bazel ignores `.bazelversion`; keeping
     them equal makes bazelisk users and the lockfile agree.
   - For each `bazel_dep`, take the newest version from
     `https://raw.githubusercontent.com/bazelbuild/bazel-central-registry/main/modules/<name>/metadata.json`.
5. **Verify** inside `nix develop` (or with bazelisk if Nix is absent):
   ```
   bazel test //...
   bazel run :hello_world
   bazel run :compile_commands     # writes compile_commands.json
   bazel mod tidy
   ```
6. **Write a README** covering setup, adding deps, and clangd, then
   `jj describe` the change. Don't push unless asked.

## Layout

| File | Purpose |
| --- | --- |
| `flake.nix` | Dev shell: `bazel_9`, `bazel-buildtools` (buildifier), pinned clang (`llvmPackages_21` as the shell's stdenv, so `CC=clang`) and matching clangd |
| `.envrc` | `use flake` for direnv |
| `MODULE.bazel` | Bzlmod deps: rules_cc, abseil-cpp, googletest, google_benchmark, compile-commands tool |
| `.bazelrc` | C++20, test output, header-parsing workaround |
| `BUILD` | `cc_library` + `cc_binary` + `cc_test` + benchmark + `:compile_commands` alias |

## Gotchas

- **No WORKSPACE.** Bazel 9 removed it; use `MODULE.bazel` only. Repo names
  are the module names: `@abseil-cpp//absl/strings`, `@googletest//:gtest_main`,
  `@google_benchmark//:benchmark_main` (not `@com_google_absl` etc.).
- **Load cc rules explicitly.** Bazel 9 has no native `cc_binary`/`cc_library`/
  `cc_test`; every BUILD file needs `load("@rules_cc//cc:cc_binary.bzl", "cc_binary")`
  and friends.
- **Don't use hedron_compile_commands.** It is unmaintained and fails to load
  on Bazel 9 (native `py_binary`/`cc_binary`). Use `wolfd_bazel_compile_commands`
  from the registry, as the template does.
- **Keep `--features=-parse_headers` and `--host_features=-parse_headers`** in
  `.bazelrc`. Abseil turns on header-parsing actions that have no source file,
  and the compile-commands generator asserts on them. Both flags are needed
  (target and exec configurations).
- **The compiler comes from Nix, not from Bazel.** The shell's stdenv is
  `llvmPackages_<N>.stdenv`, which sets `CC=clang`; Bazel's auto-detected
  toolchain uses it, with Nix's libstdc++ and glibc. `flake.lock` pins the
  exact version. Take clangd from the same set (`llvm.clang-tools`) so they
  match. Don't add `toolchains_llvm`: the user chose Nix pinning over it (one
  lock file, works on NixOS, no host glibc). Bazel must be run inside
  `nix develop`; outside it Bazel silently uses the host compiler.
- **Check the pin took effect**: `bazel run :compile_commands`, then confirm
  the compiler path in `compile_commands.json` is
  `/nix/store/...-clang-wrapper-<version>/bin/clang`.
- **`x86_64-darwin` is not a supported nixpkgs system** any more; listing it
  makes `nix flake check --all-systems` fail.
- **nixpkgs `bazel` is still Bazel 7**; ask for `pkgs.bazel_9` explicitly.
- **Flakes only see tracked files.** In a colocated jj repo new files are
  picked up automatically; in a non-colocated one Nix won't see the flake.
- `bazel run :compile_commands` also creates an `external` symlink in the
  project root; it and `compile_commands.json` are gitignored.
- clangd needs `--query-driver=/**/*` to find Nix's system headers
  (https://github.com/clangd/clangd/issues/1079).

## Adding a dependency

Look the module up on https://registry.bazel.build/, add
`bazel_dep(name = "...", version = "...")` to `MODULE.bazel`, reference it as
`@<module_name>//pkg:target`, and run `bazel mod tidy`.
