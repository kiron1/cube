//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include "cube/gloo/element_index.hpp"

namespace cube
{
namespace gloo
{
    element_index_base::element_index_base(GLenum type)
        : type_(type)
        , buffer_(buffer::element_array_buffer, buffer::static_draw)
    {
    }

    element_index_base::~element_index_base()
    {
    }

    void element_index_base::create()
    {
    }

    void element_index_base::enable()
    {
        /// TODO: implement dirty flag, to transfer only when data changed.
        switch (type_)
        {
        case GL_UNSIGNED_BYTE:
            buffer_.set(size(), static_cast<GLubyte const*>(data()));
            break;
        case GL_UNSIGNED_SHORT:
            buffer_.set(size(), static_cast<GLushort const*>(data()));
            break;
        case GL_UNSIGNED_INT:
            buffer_.set(size(), static_cast<GLuint const*>(data()));
            break;
        }

        buffer_.activate();
    }

    void element_index_base::disable()
    {
        buffer_.deactivate();
    }

    void element_index_base::destroy()
    {
        buffer_.dispose();
    }

    template <>
    element_index<GLubyte>::element_index()
        : element_index_base(GL_UNSIGNED_BYTE)
        , base()
    {
    }

    template <>
    element_index<GLushort>::element_index()
        : element_index_base(GL_UNSIGNED_SHORT)
        , base()
    {
    }

    template <>
    element_index<GLuint>::element_index()
        : element_index_base(GL_UNSIGNED_INT)
        , base()
    {
    }

    template <>
    element_index<GLubyte>::element_index(size_type size)
        : element_index_base(GL_UNSIGNED_BYTE)
        , base(size)
    {
    }

    template <>
    element_index<GLushort>::element_index(size_type size)
        : element_index_base(GL_UNSIGNED_SHORT)
        , base(size)
    {
    }

    template <>
    element_index<GLuint>::element_index(size_type size)
        : element_index_base(GL_UNSIGNED_INT)
        , base(size)
    {
    }
} // namespace gloo
} // namespace cube
