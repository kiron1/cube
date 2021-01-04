load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "display_shape",
    srcs = ["basic/display_shape.cpp"],
    deps = ["//cube:cube"],
)

cc_binary(
    name = "display_shape_rawgl",
    srcs = ["basic/display_shape_rawgl.cpp"],
    deps = ["//cube:cube"],
)

cc_binary(
    name = "model_view_projection",
    srcs = ["model_view_projection/mvp.cpp"],
    deps = ["//cube:cube"],
)