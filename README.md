
Cube
====

*A modern OpenGL C++11 visualization library for 2d and 3d data.*

This library his heavily inspired by by the Python [vispy.gloo](http://vispy.org/) library.

Documentation
-------------

- [Online documentation](http://kiron1.github.io/cube/)

Dependencies
------------

- [OpenGL](http://www.opengl.org/)
- [GLFW](https://www.glfw.org)
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

For more informations, see the [build instructions](http://kiron1.github.io/cube//cube/build.html).

