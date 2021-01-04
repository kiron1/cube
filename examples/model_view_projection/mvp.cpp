//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <iostream>
#include <iterator> // begin, end
#include <numeric>  // iota
#include <vector>

#include "cube/gloo.hpp"

// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/#the-model-view-and-projection-matrices
// https://jsantell.com/matrix-transformations/
// https://jsantell.com/model-view-projection/
// https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/WebGL_model_view_projection

//[ vertex_shader
static const std::string vertex_shader_source =
    "attribute vec3 a_position;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * view * model * vec4(a_position, 1.0);\n"
    "}\n";
//]

//[ fragment_shader
static const std::string fragment_shader_source =
    "uniform vec4 u_color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = u_color;\n"
    "}\n";
//]

//[ data
static const float model[4 * 4] = {
    // clang-format off
    +0.7f, +0.0f, +0.0f, +0.0f,
    +0.0f, +0.7f, +0.0f, +0.0f,
    +0.0f, +0.0f, +0.7f, +0.0f,
    +0.0f, +0.0f, +0.0f, +1.0f,
    // clang-format on
};

static const float view[4 * 4] = {
    // clang-format off
    +1.0f, +0.0f, +0.0f, +0.0f,
    +0.0f, +1.0f, +0.0f, +0.0f,
    +0.0f, +0.0f, +1.0f, +0.0f,
    +0.0f, +0.0f, +0.0f, +1.0f,
    // clang-format on
};

static const float position[6 * 4 * 3] = {
    // clang-format off
      // Front face
      -1.0, -1.0,  1.0,
       1.0, -1.0,  1.0,
       1.0,  1.0,  1.0,
      -1.0,  1.0,  1.0,

      // Back face
      -1.0, -1.0, -1.0,
      -1.0,  1.0, -1.0,
       1.0,  1.0, -1.0,
       1.0, -1.0, -1.0,

      // Top face
      -1.0,  1.0, -1.0,
      -1.0,  1.0,  1.0,
       1.0,  1.0,  1.0,
       1.0,  1.0, -1.0,

      // Bottom face
      -1.0, -1.0, -1.0,
       1.0, -1.0, -1.0,
       1.0, -1.0,  1.0,
      -1.0, -1.0,  1.0,

      // Right face
       1.0, -1.0, -1.0,
       1.0,  1.0, -1.0,
       1.0,  1.0,  1.0,
       1.0, -1.0,  1.0,

      // Left face
      -1.0, -1.0, -1.0,
      -1.0, -1.0,  1.0,
      -1.0,  1.0,  1.0,
      -1.0,  1.0, -1.0
    // clang-format on
};

static unsigned short outline_elements[] = {
    2, 0, 1, 2, 3, 1};

static const float face_color[4] = {
    0.2f, 1.0f, 0.4f, 1.0f};

static const float line_color[4] = {
    0.0f, 0.0f, 0.0f, 1.0f};
//]

class shape_strategy : public cube::gloo::window::strategy
{
public:
    //[ construct
    shape_strategy()
        : program_(vertex_shader_source, fragment_shader_source)
        , u_model_(program_, "model")
        , u_view_(program_, "view")
        , u_projection_(program_, "projection")
        , a_position_(program_, "a_position")
        , u_color_(program_, "u_color")
        , face_index_(4)
        , outline_index_(6)
    {
        face_index_ = {
            // clang-format off
            0,  1,  2,      0,  2,  3,    // front
            4,  5,  6,      4,  6,  7,    // back
            8,  9,  10,     8,  10, 11,   // top
            12, 13, 14,     12, 14, 15,   // bottom
            16, 17, 18,     16, 18, 19,   // right
            20, 21, 22,     20, 22, 23    // left
            // clang-format on
        };
        //std::iota(face_index_.begin(), face_index_.end(), 0);
        std::copy(std::begin(outline_elements), std::end(outline_elements), outline_index_.begin());

        a_position_ = position;
    }
    //]

private:
    void initialize()
    {
    }

    //[ display
    void display()
    {
        u_model_ = model;
        u_view_ = view;
        u_projection_ = view;

        // draw faces using face_color
        u_color_ = face_color; ///< set u_color uniform
        program_.draw(cube::gloo::program::triangle_strip, face_index_);

        // re-use program to draw outline using line_color
        //u_color_ = line_color; ///< set u_color uniform
        //program_.draw(cube::gloo::program::line_strip, outline_index_);
    }
    //]

    cube::gloo::window::strategy::command keyboard(int key)
    {
        return cube::gloo::window::strategy::command::close;
    }

    //[ close
    void close()
    {
        program_.dispose();
    }
    //]
private:
    //[ class_attributes
    cube::gloo::program program_;

    cube::gloo::uniform::mat4 u_model_;
    cube::gloo::uniform::mat4 u_view_;
    cube::gloo::uniform::mat4 u_projection_;

    cube::gloo::attribute::vec3 a_position_;

    cube::gloo::uniform::vec4 u_color_;

    cube::gloo::element_index<unsigned short> face_index_;
    cube::gloo::element_index<unsigned short> outline_index_;
    //]
};

int main(int argc, char* argv[])
{
    //[ window
    cube::gloo::window::size_type s = {400, 400};
    std::unique_ptr<cube::gloo::window::strategy> strategy(new shape_strategy);
    cube::gloo::window win(std::move(strategy), "Display Shape", s);

    win.run();
    //]

    return 0;
}
