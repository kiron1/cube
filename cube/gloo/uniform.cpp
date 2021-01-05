//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include "cube/gloo/program.hpp"
#include "cube/gloo/uniform.hpp"

namespace cube
{
namespace gloo
{
    uniform_base::uniform_base(program& p, std::string name, GLint expected_type)
        : variable(p, std::move(name), expected_type)
        , dirty_(false)
        , desc_()
    {
        program_->attach(this);
    }

    void uniform_base::create()
    {
        // nothing to do
    }

    void uniform_base::enable()
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

            if (dirty_)
            {
                assert(desc->type == expected_type());
                assert(desc->size == 1 && "Arrays are not (yet) supported");
                transfer();
                assert(glGetError() == GL_NO_ERROR);
                dirty_ = false;
            }
        }
    }

    void uniform_base::disable()
    {
    }

    void uniform_base::destroy()
    {
        if (program_)
        {
            program_->detach(this);
            program_ = nullptr;
            dirty_ = false;
            desc_.reset();
        }
    }

    template <>
    basic_uniform_vector<float, 1>::basic_uniform_vector(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT)
    {
    }

    template <>
    void basic_uniform_vector<float, 1>::transfer() const
    {
        glUniform1fv(handle(), 1, data_);
    }

    template <>
    basic_uniform_vector<float, 2>::basic_uniform_vector(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_VEC2)
    {
    }

    template <>
    void basic_uniform_vector<float, 2>::transfer() const
    {
        glUniform2fv(handle(), 1, data_);
    }

    template <>
    basic_uniform_vector<float, 3>::basic_uniform_vector(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_VEC3)
    {
    }

    template <>
    void basic_uniform_vector<float, 3>::transfer() const
    {
        glUniform3fv(handle(), 1, data_);
    }

    template <>
    basic_uniform_vector<float, 4>::basic_uniform_vector(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_VEC4)
    {
    }

    template <>
    void basic_uniform_vector<float, 4>::transfer() const
    {
        glUniform4fv(handle(), 1, data_);
    }

    template <>
    basic_uniform_matrix<float, 2>::basic_uniform_matrix(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_MAT2)
    {
    }

    template <>
    void basic_uniform_matrix<float, 2>::transfer() const
    {
        glUniformMatrix2fv(handle(), 1, GL_FALSE, data_);
    }

    template <>
    basic_uniform_matrix<float, 3>::basic_uniform_matrix(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_MAT3)
    {
    }

    template <>
    void basic_uniform_matrix<float, 3>::transfer() const
    {
        glUniformMatrix3fv(handle(), 1, GL_FALSE, data_);
    }

    template <>
    basic_uniform_matrix<float, 4>::basic_uniform_matrix(program& p, std::string name)
        : uniform_base(p, std::move(name), GL_FLOAT_MAT4)
    {
    }

    template <>
    void basic_uniform_matrix<float, 4>::transfer() const
    {
        glUniformMatrix4fv(handle(), 1, GL_FALSE, data_);
    }
} // namespace gloo
} // namespace cube
