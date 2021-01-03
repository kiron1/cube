//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <cube/gloo/buffer.hpp>
#include <cube/gloo/globject.hpp>

namespace cube
{
namespace gloo
{
    class program;

    class element_index_base : public globject<element_index_base>
    {
        friend class program;

    public:
        virtual void const* data() const = 0;
        virtual std::size_t size() const = 0;

        ~element_index_base();

    protected:
        explicit element_index_base(GLenum type);

    private:
        GLenum type() const { return type_; }

    private:
        friend class globject<element_index_base>;
        void create();
        void enable();
        void disable();
        void destroy();

    private:
        GLenum const type_;
        buffer buffer_;
    };

    /// Element index proxy class
    /**
     * The element_index contains a array to store the indices.
     * Upon drawing using a element_index the array is transfered
     * to the GPU.
     */
    template <typename T>
    class element_index
        : public element_index_base,
          private std::vector<T>
    {
        using base = std::vector<T>;

    public:
        using size_type = typename base::size_type;
        using value_type = typename base::value_type;
        using pointer = typename base::pointer;
        using const_pointer = typename base::const_pointer;
        using reference = typename base::reference;
        using const_reference = typename base::const_reference;
        using iterator = typename base::iterator;
        using const_iterator = typename base::const_iterator;
        using reverse_iterator = typename base::reverse_iterator;
        using const_reverse_iterator = typename base::const_reverse_iterator;

        using base::at;
        using base::back;
        using base::begin;
        using base::capacity;
        using base::clear;
        using base::end;
        using base::front;
        using base::rbegin;
        using base::rend;
        using base::operator=;
        using base::operator[];
        using base::assign;
        using base::push_back;

        /// Construct an empty array.
        element_index();

        /// Construct an array of a specific \a size.
        explicit element_index(size_type size);

        ~element_index();

        void const* data() const { return base::data(); }
        std::size_t size() const { return base::size(); }

    private:
    };

    template <typename T>
    element_index<T>::~element_index()
    {
    }
} // namespace gloo
} // namespace cube
