workspace(name = "cube")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "boost",
    urls = ["https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.bz2"],
    sha256 = "953db31e016db7bb207f11432bef7df100516eeb746843fa0486a222e3fd49cb",
    build_file = "@//bazel:boost.BUILD",
    strip_prefix = "boost_1_75_0",
)


http_archive(
    name = "glfw",
    urls = ["https://github.com/glfw/glfw/archive/3.3.2.zip"],
    sha256 = "33c6bfc422ca7002befbb39a7a60dc101d32c930ee58be532ffbcd52e9635812",
    build_file = "@//bazel:glfw.BUILD",
    strip_prefix = "glfw-3.3.2",
)