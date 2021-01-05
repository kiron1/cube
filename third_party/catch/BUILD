load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "catch",
    hdrs = glob(["catch.hpp"]),
    includes = ["."],
    defines = ["CATCH_CONFIG_MAIN=1"],
)
