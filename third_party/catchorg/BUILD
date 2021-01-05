load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "catch2",
    hdrs = ["catch2/catch.hpp"],
    includes = ["."],
)

cc_library(
    name = "catch2_with_main",
    hdrs = ["catch2/catch.hpp"],
    includes = ["."],
    defines = ["CATCH_CONFIG_MAIN=1"],
)
