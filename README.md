# nix-bazel-abslcpp

zero bullshit template for using nix, bazel, and abslcpp.

get started with writing cpp with absl in a couple minutes without touching a
single makefile.

## Setup

```
$ nix develop
$ bazel run :hello_world
$ bazel test //...
$ bazel run -c opt :greeting_benchmark
```

If you use [direnv](https://direnv.net/), `direnv allow` loads the dev shell
automatically.

The dev shell provides `bazel`, `clang`, `clangd`, and `buildifier`.

The compiler is pinned by nix: `flake.nix` selects `llvmPackages_21` and
`flake.lock` fixes the exact release (currently clang 21.1.8). The shell sets
`CC=clang` and Bazel builds with that compiler, along with nix's libstdc++ and
glibc, so nothing from the host's `/usr` goes into a build. clangd comes from
the same LLVM package set, so the editor and the build always agree.

Always run Bazel from inside the dev shell. Outside it Bazel falls back to
whatever compiler the host has.

## Dependencies

Dependencies are managed with [Bzlmod](https://bazel.build/external/module) in
`MODULE.bazel`. Find modules on the [Bazel Central Registry](https://registry.bazel.build/),
add a `bazel_dep`, then run `bazel mod tidy`.

To run abseil's own tests:

```
$ bazel test --test_tag_filters=-benchmark @abseil-cpp//...
```

To update nixpkgs (and with it bazel and clang), run `nix flake update` and set
`.bazelversion` to match `bazel --version`. To move to a new LLVM major version,
change `llvmPackages_21` in `flake.nix`.

## LSP Support

If you're using the clangd language server and want auto completions to show up
properly run:

```
$ bazel run :compile_commands
```

Re-run it after adding files or changing dependencies.

C++20 is enabled in `.bazelrc`; change `-std=c++20` there to use another
standard.

Also, there is a [clangd bug](https://github.com/clangd/clangd/issues/1079)
that requires you to do a work around, by adding this flag clangd:
`--query-driver=/**/*`

For example if you use `lspconfig` for neovim, your config may look like this:

```
require("lspconfig").clangd.setup({
  cmd = { "clangd", "--query-driver=/**/*" },
  ...
})
```

## Claude Code skill

`.claude/skills/nix-bazel-cpp/SKILL.md` teaches [Claude Code](https://claude.com/claude-code)
how to scaffold a new project from this template, including the gotchas.
