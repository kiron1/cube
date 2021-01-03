//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <iostream>
#include <iterator> // begin, end
#include <numeric>  // iota
#include <vector>

#include <cube/gloo.hpp>

//[ vertex_shader
static const std::string vertex_shader_source =
    "// Attributes\n"
    "// ------------------------------------\n"
    "attribute vec3 a_position;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_position, 1.0);\n"
    "}\n";
//]

//[ fragment_shader
static const std::string fragment_shader_source =
    "// Uniforms\n"
    "// ------------------------------------\n"
    "uniform vec4 u_color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = u_color;\n"
    "}\n";
//]

//[ data
static const float position[4 * 3] = {
    // clang-format off
    -0.8f, -0.8f, 0.0f,
    +0.7f, -0.7f, 0.0f,
    -0.7f, +0.7f, 0.0f,
    +0.8f, +0.8f, 0.0f,
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
        , a_position_(program_, "a_position")
        , u_color_(program_, "u_color")
        , face_index_(4)
        , outline_index_(6)
    {
        std::iota(face_index_.begin(), face_index_.end(), 0);
        std::copy(std::begin(outline_elements), std::end(outline_elements), outline_index_.begin());

        a_position_ = position; ///< set a_position attribute
    }
    //]

private:
    void initialize()
    {
    }

    //[ display
    void display()
    {
        // draw faces using face_color
        u_color_ = face_color; ///< set u_color uniform
        program_.draw(cube::gloo::program::triangle_strip, face_index_);

        // re-use program to draw outline using line_color
        u_color_ = line_color; ///< set u_color uniform
        program_.draw(cube::gloo::program::line_strip, outline_index_);
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

    cube::gloo::attribute::vec3 a_position_; // 'attribute vec3 a_position' proxy

    cube::gloo::uniform::vec4 u_color_; // 'uniform vec4 u_color' proxy

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
