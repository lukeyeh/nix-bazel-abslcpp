# nix-bazel-abslcpp

zero bullshit template for using nix, bazel, and abslcpp.

get started with writing cpp with absl in a couple minutes without touching a
single makefile.

## Setup 

```
$ nix-develop 
$ bazel test --test_tag_filters=-benchmark @com_google_absl//...
$ bazel run :hello_world
```

## LSP Support

If you're using the clangd language server and want auto completions to show up
properly run:

```
$ bazel run @hedron_compile_commands//:refresh_all
```

If you auto-completions aren't showing up in test files remember to add the `--compilation_mode=dbg`
flag to the above.

For cpp20, first add `build --cxxopt='-std=c++20'` to your `.bazelrc` file.

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


