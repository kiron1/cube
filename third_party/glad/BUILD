load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "glad",
    hdrs = glob(["include/**/*.h"]),
    srcs = glob(["src/**/*.c"]),
    strip_include_prefix = "include",
)
