load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_library(
    name = "greeting",
    srcs = ["greeting.cc"],
    hdrs = ["greeting.h"],
    deps = ["@abseil-cpp//absl/strings"],
)

cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cc"],
    deps = [":greeting"],
)

cc_test(
    name = "greeting_test",
    srcs = ["greeting_test.cc"],
    deps = [
        ":greeting",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "greeting_benchmark",
    testonly = True,
    srcs = ["greeting_benchmark.cc"],
    deps = [
        ":greeting",
        "@google_benchmark//:benchmark_main",
    ],
)

alias(
    name = "compile_commands",
    actual = "@wolfd_bazel_compile_commands//:generate_compile_commands",
)
