//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include "cube/gloo/shader.hpp"

namespace
{
bool shader_compile(const GLint shader, std::string const& source)
{
    const GLchar* src[] = {source.c_str()};
    const GLint srclen[] = {static_cast<GLint>(source.size())};
    GLint status;

    glShaderSource(shader, 1UL, src, srclen);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    return static_cast<bool>(status);
}

std::string shader_info_log(const GLint shader)
{
    std::string result;
    GLint log_length;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    result.resize(log_length);
    glGetShaderInfoLog(shader, result.size(), nullptr, &result[0]);
    return result;
}
} // namespace

namespace cube
{
namespace gloo
{
    detail::shader::shader(GLuint u)
        : type_(u)
        , source_()
        , compiled_(false)
    {
    }

    void detail::shader::code(std::string source)
    {
        source_ = std::move(source);
        compiled_ = false;
    }

    void detail::shader::create()
    {
        const auto h = glCreateShader(type_);
        assert(glGetError() == GL_NO_ERROR);
        assert(h != 0);
        handle(h);
    }

    void detail::shader::enable()
    {
        assert(!source_.empty());

        if (!compiled_)
        {
            const auto compile_status = shader_compile(handle(), source_);

            if (!compile_status)
            {
                const auto log = shader_info_log(handle());
            }
            assert(compile_status);
            compiled_ = true;
        }
    }

    void detail::shader::disable()
    {
    }

    void detail::shader::destroy()
    {
        glDeleteShader(handle());
        assert(glGetError() == GL_NO_ERROR);
    }

    vertex_shader::vertex_shader()
        : shader(GL_VERTEX_SHADER)
    {
    }

    vertex_shader::vertex_shader(std::string src)
        : shader(GL_VERTEX_SHADER)
    {
        code(std::move(src));
    }

    fragment_shader::fragment_shader()
        : shader(GL_FRAGMENT_SHADER)
    {
    }

    fragment_shader::fragment_shader(std::string src)
        : shader(GL_FRAGMENT_SHADER)
    {
        code(std::move(src));
    }
} // namespace gloo
} // namespace cube
