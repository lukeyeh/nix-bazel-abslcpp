# nix-bazel-abslcpp

quickstart template for using nix, bazel, and abslcpp

## Setup 

```
$ nix-develop 
$ bazel test --test_tag_filters=-benchmark @com_google_absl//...
$ bazel run :hello_world
```

If you're using the clangd language server and want auto completions to show up
properly run:

```
$ bazel run @hedron_compile_commands//:refresh_all
```
