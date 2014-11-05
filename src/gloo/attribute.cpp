//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include <cube/gloo/program.hpp>
#include <cube/gloo/attribute.hpp>

namespace cube
{
namespace gloo
{
    attribute_base::attribute_base(program& p, std::string name, GLint expected_type, std::size_t dim)
        : variable(p, std::move(name), expected_type)
        , buffer_(buffer::array_buffer, buffer::static_draw)
        , size_(0)
        , desc_()
        , generic_(false)
        , dim_(dim)
    {
        program_->attach(this);
    }

    void attribute_base::create()
    {
        buffer_.create();
    }

    void attribute_base::enable()
    {
        if (desc_.expired())
        {
            // reconnect and make sure it is the right expected type.
            desc_ = program_->variable(this);
        }

        // if we are still expired, the variable is not available in the program.
        auto desc = desc_.lock();
        if (desc)
        {
            handle(desc->location);

            if (generic_)
            {
                glDisableVertexAttribArray(handle());
                // TODO: call glVertexAttribXf
            }
            else
            {
                const GLint size = dim_;
                const GLenum type = GL_FLOAT; // TODO: do not hardcode float, support other types too
                const GLboolean normalized = GL_FALSE;
                const GLsizei stride = sizeof(GLfloat) * dim_;
                const GLvoid* const offset = nullptr;

                buffer_.enable();
                glEnableVertexAttribArray(handle());
                assert(glGetError() == GL_NO_ERROR);
                glVertexAttribPointer(handle(), size, type, normalized, stride, offset);
                assert(glGetError() == GL_NO_ERROR);
            }
        }
    }

    void attribute_base::disable()
    {
        glDisableVertexAttribArray(handle());
        buffer_.disable();
    }

    void attribute_base::destroy()
    {
        if (!generic_)
        {
            buffer_.destroy();
        }
    }

    template <>
    basic_attribute<float, 1>::basic_attribute(program& p, std::string name)
        : attribute_base(p, std::move(name), GL_FLOAT, 1UL)
    {
    }

    template <>
    basic_attribute<float, 2>::basic_attribute(program& p, std::string name)
        : attribute_base(p, std::move(name), GL_FLOAT_VEC2, 2UL)
    {
    }

    template <>
    basic_attribute<float, 3>::basic_attribute(program& p, std::string name)
        : attribute_base(p, std::move(name), GL_FLOAT_VEC3, 3UL)
    {
    }

    template <>
    basic_attribute<float, 4>::basic_attribute(program& p, std::string name)
        : attribute_base(p, std::move(name), GL_FLOAT_VEC4, 4UL)
    {
    }
}
}
