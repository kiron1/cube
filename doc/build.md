
Build {#build}
=====

Dependencies
------------

- [OpenGL](http://www.opengl.org/)
- [GLEW](http://www.glew.org/)
- [FreeGLUT](http://freeglut.sf.net)
- [Boost](http://boost.org/)
- C++11 compiler
- [cmake](http://www.cmake.org/) 3.0 or newer
- optional [Doxygen](http://www.doxygen.org/) to generate documentation

Configure
---------

    cmake -H. -B_build

### Configure options

- [CMAKE_BUILD_TYPE](http://www.cmake.org/cmake/help/v3.0/variable/CMAKE_BUILD_TYPE.html) - Change build type.
- [CMAKE_INSTALL_PREFIX](http://www.cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) - Change installation location.

To change some options, append `-DOPTION=Value` (e.g.
`-DCMAKE_BUILD_TYPE=Debug` to generate the project to build in debug mode).

Compile
-------

    cmake --build _build --config Release

Install
-------

    cmake --build _build --config Release --target install

