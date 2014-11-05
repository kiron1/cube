
CubeGloo {#mainpage}
========

*A modern OpenGL C++11 visualization library for 2d and 3d data.*

This library his heavily inspired by by the Python [vispy.gloo](http://vispy.org/) library.

Documentation
-------------

- \subpage introduction
- \subpage examples
- \subpage developers
- \subpage references

Dependencies
------------

- [OpenGL](http://www.opengl.org/)
- [GLEW](http://www.glew.org/)
- [FreeGLUT](http://freeglut.sf.net)
- [Boost](http://boost.org/)
- C++11 compiler
- [cmake](http://www.cmake.org/) 3.0 or newer
- optional [Doxygen](http://www.doxygen.org/) to generate documentation


Build
-----

- Configure the project:

      cmake -H. -B_build

- Build the project:

      cmake --build _build --config Release

- Install library:

      cmake --build _build --config Release --target install


Configure options
-----------------

- [CMAKE_BUILD_TYPE](http://www.cmake.org/cmake/help/v3.0/variable/CMAKE_BUILD_TYPE.html) - Change build type.
- [CMAKE_INSTALL_PREFIX](http://www.cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) - Change installation location.
