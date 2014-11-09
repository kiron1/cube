//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>
#include <iostream>
#include <vector>

#include <cube/gloo/opengl.hpp>
#include <cube/gloo/window.hpp>

static const std::string vertex_shader_source =
    "// Attributes\n"
    "// ------------------------------------\n"
    "attribute vec3 a_position;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_position, 1.0);\n"
    "}\n";

static const std::string fragment_shader_source =
    "// Uniforms\n"
    "// ------------------------------------\n"
    "uniform vec4 u_color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = u_color;\n"
    "}\n";

static const float color[4] = {
    0.2f, 1.0f, 0.4f, 1.0f
};

static const float position[4*3] = {
    -0.8f, -0.8f, 0.0f,
    +0.7f, -0.7f, 0.0f,
    -0.7f, +0.7f, 0.0f,
    +0.8f, +0.8f, 0.0f,
};

static const GLushort elements[] = {
    0, 1, 2, 3
};

//[ helpers
static GLuint make_buffer(
    GLenum target,
    const void *data,
    GLsizei size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
    return buffer;
}
static std::string info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC shader_iv_proc,
    PFNGLGETSHADERINFOLOGPROC shader_infolog_proc
)
{
    GLint len;
    std::string log;

    shader_iv_proc(object, GL_INFO_LOG_LENGTH, &len);
    log.resize(len);
    shader_infolog_proc(object, len, NULL, &log[0]);
    return log;
}

static GLuint make_shader(GLenum type, std::string const& source)
{
    const GLchar* src[] = { source.c_str() };
    const GLint srclen[] = { static_cast<GLint>(source.size()) };
    GLuint shader;
    GLint shader_ok;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, src, srclen);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        std::cerr
                << "Failed to compile shader:\n"
                << info_log(shader, glGetShaderiv, glGetShaderInfoLog)
                << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        std::cerr
                << "Failed to link shader program:\n"
                << info_log(program, glGetProgramiv, glGetProgramInfoLog)
                << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    return program;
}
//]

class shape_strategy : public cube::gloo::window::strategy
{
public:
    shape_strategy()
    {
    }

private:
    //[ initialize
    void initialize()
    {
        vertex_shader_ = make_shader(
            GL_VERTEX_SHADER,
            vertex_shader_source
        );
        assert(vertex_shader_ != 0);
        assert(glGetError() == GL_NO_ERROR);

        fragment_shader_ = make_shader(
            GL_FRAGMENT_SHADER,
            fragment_shader_source
        );
        assert(fragment_shader_ != 0);
        assert(glGetError() == GL_NO_ERROR);

        program_ = make_program(vertex_shader_, fragment_shader_);
        assert(program_ != 0);
        assert(glGetError() == GL_NO_ERROR);

        color_uniform_ = glGetUniformLocation(program_, "u_color");
        assert(glGetError() == GL_NO_ERROR);

        position_attrib_ = glGetAttribLocation(program_, "a_position");
        assert(glGetError() == GL_NO_ERROR);

        position_buffer_ = make_buffer(
            GL_ARRAY_BUFFER,
            position,
            sizeof(position)
        );
        assert(position_buffer_ != 0);
        assert(glGetError() == GL_NO_ERROR);

        element_buffer_ = make_buffer(
            GL_ELEMENT_ARRAY_BUFFER,
            elements,
            sizeof(elements)
        );
        assert(position_buffer_ != 0);
        assert(glGetError() == GL_NO_ERROR);
    }
    //]

    //[ display_gl
    void display()
    {
        glUseProgram(program_);

        glUniform4fv(color_uniform_, 1UL, color);

        glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
        glEnableVertexAttribArray(position_attrib_);
        glVertexAttribPointer(
            position_attrib_,     /* attribute */
            3,                    /* size */
            GL_FLOAT,             /* type */
            GL_FALSE,             /* normalized? */
            sizeof(GLfloat)*3,    /* stride */
            (void*)0              /* array buffer offset */
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
        glDrawElements(
            GL_TRIANGLE_STRIP,  /* mode */
            4,                  /* count */
            GL_UNSIGNED_SHORT,  /* type */
            (void*)0            /* element array buffer offset */
        );

        assert(glGetError() == GL_NO_ERROR);

        glDisableVertexAttribArray(position_attrib_);
    }
    //]

    void keyboard(unsigned char key, int w, int h)
    {
        leave();
    }

    //[ close_gl
    void close()
    {
        glDeleteProgram(program_);
        glDeleteShader(vertex_shader_);
        glDeleteShader(fragment_shader_);
        glDeleteBuffers(1, &position_buffer_);
    }
    //]

private:
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint program_;
    GLuint position_buffer_;
    GLuint element_buffer_;
    GLuint position_attrib_;
    GLuint color_uniform_;
};

int main(int argc, char* argv[])
{
    cube::gloo::window::size_type s = { 400, 400 };
    std::unique_ptr<cube::gloo::window::strategy> strategy(new shape_strategy);
    cube::gloo::window win(std::move(strategy), "Display Shape RawGL", s);

    win.run();

    return 0;
}
