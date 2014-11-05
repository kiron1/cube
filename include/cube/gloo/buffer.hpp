//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <functional>
#include <queue>

#include <cube/gloo/globject.hpp>

namespace cube
{
namespace gloo
{
    class buffer : public globject<buffer, with_handle, with_target>
    {
    public:
        enum target_type
        {
            array_buffer = GL_ARRAY_BUFFER,
            element_array_buffer = GL_ELEMENT_ARRAY_BUFFER
        };

        enum usage_type
        {
            stream_draw = GL_STREAM_DRAW,
            stream_read = GL_STREAM_READ,
            stream_copy = GL_STREAM_COPY,
            static_draw = GL_STATIC_DRAW,
            static_read = GL_STATIC_READ,
            static_copy = GL_STATIC_COPY,
            dynamic_draw = GL_DYNAMIC_DRAW,
            dynamic_read = GL_DYNAMIC_READ,
            dynamic_copy = GL_DYNAMIC_COPY
        };

        buffer(target_type t, usage_type usage);

        template <typename T>
        void set(std::size_t size, const T* data);

        template <typename T>
        void set(std::size_t offset, std::size_t size, const T* data);

    private:
        friend class globject<buffer, with_handle, with_target>;
        friend class attribute_base;
        void create();
        void enable();
        void disable();
        void destroy();

    private:
        struct buffer_view
        {
            GLintptr offset;
            GLsizei size;
            const void* data;
        };

        const GLenum usage_;
        std::size_t size_;
        std::queue<buffer_view> transfer_queue_;
    };

    template <typename T>
    void buffer::set(std::size_t size, const T* data)
    {
        set(0U, size, data);
    }

    template <typename T>
    void buffer::set(std::size_t offset, std::size_t size, const T* data)
    {
        GLintptr const bufoffset = static_cast<GLintptr>(sizeof(T) * offset);
        GLsizei const bufsize = static_cast<GLsizei>(sizeof(T) * size);

        if (bufoffset + bufsize > size_)
        {
            //transfer_queue_.clear();
        }
        transfer_queue_.push(buffer_view{bufoffset, bufsize, data});
    }
}
}
