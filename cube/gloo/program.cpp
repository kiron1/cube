//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include <algorithm>
#include <vector>

#include <boost/iterator/indirect_iterator.hpp>

#include <cube/gloo/program.hpp>

namespace
{
void detach_shaders(const GLint program)
{
    GLint count;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
    if (count > 0)
    {
        std::vector<GLuint> shaders(static_cast<std::size_t>(count), 0);
        glGetAttachedShaders(program, shaders.size(), 0, &shaders[0]);
        for (auto u : shaders)
        {
            glDetachShader(program, u);
        }
    }
}

template <typename Container>
void attach_shaders(const GLint program, Container& shaders)
{
    assert(glIsProgram(program));
    for (auto& u : shaders)
    {
        u.activate();
        assert(glIsShader(u.handle()));
        glAttachShader(program, u.handle());
        assert(glGetError() == GL_NO_ERROR);
    }
}

bool program_link(const GLint program)
{
    GLint status;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    return static_cast<bool>(status);
}

bool program_validate(const GLint program)
{
    GLint status;
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    return static_cast<bool>(status);
}

std::string program_info_log(const GLint program)
{
    std::string result;
    GLint log_length;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    result.resize(log_length);
    glGetProgramInfoLog(program, result.size(), nullptr, &result[0]);
    return result;
}
} // namespace

namespace cube
{
namespace gloo
{
    program::program()
        : need_link_(false)
    {
    }

    program::program(vertex_shader vertex, fragment_shader fragment)
        : need_link_(false)
    {
        attach(vertex);
        attach(fragment);
    }

    program::program(std::string vertex, std::string fragment)
        : program(vertex_shader(vertex), fragment_shader(fragment))
    {
    }

    void program::attach(vertex_shader const& shader)
    {
        vertex_shaders_.push_back(shader);
        need_link_ = true;
    }

    void program::attach(fragment_shader const& shader)
    {
        fragment_shaders_.push_back(shader);
        need_link_ = true;
    }

    void program::draw(draw_mode mode)
    {
        assert(attached_attributes_.size() > 0); // TODO: also make sure it is located!
        const GLint first = 0;
        const GLint count = (*attached_attributes_.begin())->size();

        activate();
        glDrawArrays(mode, first, count);
        assert(glGetError() == GL_NO_ERROR);
        deactivate();
    }

    void program::draw(draw_mode mode, element_index_base& indices)
    {
        activate();
        indices.activate();
        glDrawElements(
            mode,
            indices.size(),
            indices.type(),
            (void*)0);
        indices.deactivate();
        deactivate();
    }

    void program::attach(attribute_base* symbol)
    {
        attached_attributes_.insert(symbol);
    }

    void program::attach(uniform_base* symbol)
    {
        attached_uniforms_.insert(symbol);
    }

    std::weak_ptr<detail::attribute_desc> program::variable(attribute_base* symbol)
    {
        std::weak_ptr<detail::attribute_desc> result;
        const auto iter = attribute_locations_.find(symbol->name());
        if (iter != attribute_locations_.end())
        {
            attached_attributes_.insert(symbol);
            result = iter->second;
        }
        return result;
    }

    std::weak_ptr<detail::uniform_desc> program::variable(uniform_base* symbol)
    {
        std::weak_ptr<detail::uniform_desc> result;
        const auto iter = uniform_locations_.find(symbol->name());
        if (iter != uniform_locations_.end())
        {
            attached_uniforms_.insert(symbol);
            result = iter->second;
        }
        return result;
    }

    void program::detach(attribute_base* symbol)
    {
        attached_attributes_.erase(symbol);
    }

    void program::detach(uniform_base* symbol)
    {
        attached_uniforms_.erase(symbol);
    }

    program::attribute_table program::active_attributes()
    {
        typedef program::attribute_table result_type;
        result_type result;
        GLint max_length;
        GLint count;

        glGetProgramiv(handle(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
        glGetProgramiv(handle(), GL_ACTIVE_ATTRIBUTES, &count);

        result.reserve(static_cast<std::size_t>(count));
        for (std::size_t k = 0; k != static_cast<std::size_t>(count); ++k)
        {
            std::string name(max_length, '\0');
            GLsizei len;
            auto desc = std::make_shared<std::pointer_traits<result_type::value_type::second_type>::element_type>();

            glGetActiveAttrib(handle(), k, name.size(), &len, &desc->size, &desc->type, &name[0]);
            assert(glGetError() == GL_NO_ERROR);
            name.resize(len);

            desc->location = glGetAttribLocation(handle(), name.c_str());
            assert(glGetError() == GL_NO_ERROR);
            assert(desc->location != -1);

            result.insert(std::make_pair(std::move(name), std::move(desc)));
        }
        return result;
    }

    program::uniform_table program::active_uniforms()
    {
        typedef program::uniform_table result_type;
        result_type result;
        GLint max_length;
        GLint count;

        glGetProgramiv(handle(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
        glGetProgramiv(handle(), GL_ACTIVE_UNIFORMS, &count);

        result.reserve(static_cast<std::size_t>(count));
        for (std::size_t k = 0; k != static_cast<std::size_t>(count); ++k)
        {
            std::string name(max_length, '\0');
            GLsizei len;
            auto desc = std::make_shared<std::pointer_traits<result_type::value_type::second_type>::element_type>();

            glGetActiveUniform(handle(), k, name.size(), &len, &desc->size, &desc->type, &name[0]);
            assert(glGetError() == GL_NO_ERROR);
            name.resize(len);

            desc->location = glGetUniformLocation(handle(), name.c_str());
            assert(glGetError() == GL_NO_ERROR);

            result.insert(std::make_pair(std::move(name), std::move(desc)));
        }
        return result;
    }

    void program::create()
    {
        const auto h = glCreateProgram();
        assert(h != 0);
        handle(h);
    }

    void program::enable()
    {
        using std::begin;
        using std::end;

        assert(!vertex_shaders_.empty());
        assert(!fragment_shaders_.empty());

        const auto v = std::any_of(begin(vertex_shaders_), end(vertex_shaders_), [](vertex_shader const& u) {
            return !u.compiled();
        });
        const auto f = std::any_of(begin(fragment_shaders_), end(fragment_shaders_), [](fragment_shader const& u) {
            return !u.compiled();
        });

        const auto need_build = v || f;

        bool did_build = false;

        if (need_build || need_link_)
        {
            detach_shaders(handle());

            attach_shaders(handle(), vertex_shaders_);
            attach_shaders(handle(), fragment_shaders_);

            const auto link_status = program_link(handle());

            if (!link_status)
            {
                const auto log = program_info_log(handle());
            }
            assert(link_status);

            attribute_locations_ = active_attributes();
            uniform_locations_ = active_uniforms();
            need_link_ = false;
            did_build = true;
        }

        glUseProgram(handle());
        assert(glGetError() == GL_NO_ERROR);

        activate_all(boost::make_indirect_iterator(attached_attributes_.begin()), boost::make_indirect_iterator(attached_attributes_.end()));
        activate_all(boost::make_indirect_iterator(attached_uniforms_.begin()), boost::make_indirect_iterator(attached_uniforms_.end()));

        if (did_build)
        {
            const auto validate_status = program_validate(handle());
            const auto log = program_info_log(handle());
            assert(validate_status);
        }
    }

    void program::disable()
    {
        using std::begin;
        using std::end;

        deactivate_all(boost::make_indirect_iterator(attached_attributes_.begin()), boost::make_indirect_iterator(attached_attributes_.end()));
        deactivate_all(boost::make_indirect_iterator(attached_uniforms_.begin()), boost::make_indirect_iterator(attached_uniforms_.end()));

        glUseProgram(0);
    }

    void program::destroy()
    {
        detach_shaders(handle());

        dispose_all(begin(vertex_shaders_), end(vertex_shaders_));
        dispose_all(begin(fragment_shaders_), end(fragment_shaders_));

        dispose_all(boost::make_indirect_iterator(attached_attributes_.begin()), boost::make_indirect_iterator(attached_attributes_.end()));
        dispose_all(boost::make_indirect_iterator(attached_uniforms_.begin()), boost::make_indirect_iterator(attached_uniforms_.end()));

        assert(glIsProgram(handle()));
        glDeleteProgram(handle());
        assert(glGetError() == GL_NO_ERROR);
        handle(0);
    }
} // namespace gloo
} // namespace cube
