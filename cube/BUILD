load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cube",
    hdrs = glob(["**/*.hpp"]),
    srcs = glob(["**/*.cpp"], exclude=["**/*_test.cpp"]),
    deps = [
        "@boost//:boost",
        "@glfw//:glfw",
        "//third_party/glad:glad",
    ]
)

cc_test(
    name = "mat_test",
    srcs = ["mat_test.cpp"],
    deps = [
        ":cube",
        "//third_party/catch:catch",
    ],
)

cc_test(
    name = "globject_test",
    srcs = ["gloo/globject_test.cpp"],
    deps = [
        ":cube",
        "//third_party/catch:catch",
    ],
)