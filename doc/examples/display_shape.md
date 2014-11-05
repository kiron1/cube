
Display Shape {#display_shape}
=============

\note The idea of this example is from
[vispy](http://vispy.org/examples/basics/gloo/display_shape.html).

This example is provided in two version. One version uses the raw
OpenGL calls, the other uses the cube::gloo higher level API.

This example demontrates how to draw two colored triangles on the screen.

OpenGL Shader code
------------------

The used OpenGL shaders are the same for both versions:

<table>
<tr><th>Vertex shader</th><th>Fragment shader</th></tr>
<tr>
<td>
\snippet display_shape_rawgl.cpp vertex_shader
<td>
\snippet display_shape_rawgl.cpp fragment_shader
</td>
</tr>
</table>

OpenGL data
-----------

The uniforms and vertex attributes are also the same for both bersions:

\snippet display_shape_rawgl.cpp data


Helper functions for raw OpenGL
-------------------------------

First introduces some helper functions used in the raw OpenGL example:

<table>
<tr><th>raw OpenGL</th><th>\c cube::gloo</th></tr>
<tr>
<td>
\snippet display_shape_rawgl.cpp helpers
</td>
<td>
\note The \c cube::gloo version does not need any helpers, the library is the helper.
</td>
</tr>
</table>
Initialize
----------

The raw OpenGL objects must be create when an OpenGL context is available.
<table>
<tr><th>raw OpenGL</th><th>\c cube::gloo</th></tr>
<tr>
<td>
\snippet display_shape_rawgl.cpp initialize
</td>
<td>
\snippet display_shape.cpp construct
With the following class attributes:
\snippet display_shape.cpp class_attributes
\note
Since the OpenGL objects in \c cube::gloo are lazy, we can create them in
the constructor of the class although now OpenGL context is available yet.
</td>
</tr>
</table>

Display
-------
<table>
<tr><th>raw OpenGL</th><th>\c cube::gloo</th></tr>
<tr>
<td>
\snippet display_shape_rawgl.cpp display
</td>
<td>
\snippet display_shape.cpp display
\note
The \c cube::gloo version draws the faces and outline of the triangles,
the raw OpenGL version draws just the faces.
</td>
</tr>
</table>
