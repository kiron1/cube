//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>
#include <cstddef>

#include "cube/gloo/buffer.hpp"

namespace cube
{
namespace gloo
{
    buffer::buffer(target_type t, usage_type usage)
        : usage_(usage)
        , size_(0)
    {
        target(static_cast<GLint>(t));
    }

    void buffer::create()
    {
        const std::size_t N = 1UL;
        GLuint buffers[N];
        glGenBuffers(N, buffers);
        assert(glGetError() == GL_NO_ERROR);
        assert(buffers[0] != 0);
        handle(buffers[0]);
    }

    void buffer::enable()
    {
        assert(target() == GL_ARRAY_BUFFER || target() == GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(target(), handle());
        assert(glIsBuffer(handle()));
        assert(glGetError() == GL_NO_ERROR);

        while (!transfer_queue_.empty())
        {
            buffer_view const& bv = transfer_queue_.front();
            const std::size_t data_size = bv.offset + bv.size;

            if (data_size > size_)
            {
                size_ = data_size;
                glBufferData(target(), data_size, bv.data, usage_);
            }
            else
            {
                glBufferSubData(target(), bv.offset, bv.size, bv.data);
            }
            transfer_queue_.pop();
        }
    }

    void buffer::disable()
    {
        glBindBuffer(target(), 0);
    }

    void buffer::destroy()
    {
        const std::size_t N = 1UL;
        GLuint buffers[N] = {handle()};
        glDeleteBuffers(N, buffers);
        assert(glGetError() == GL_NO_ERROR);
    }
} // namespace gloo
} // namespace cube
