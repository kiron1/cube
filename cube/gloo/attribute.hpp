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

#include "cube/gloo/buffer.hpp"
#include "cube/gloo/detail/variable.hpp"
#include "cube/gloo/globject.hpp"

namespace cube
{
namespace gloo
{
    class program;

    /// Attribute proxy of an OpenGL program.
    /**
     * \note
     * The attribute does \b not store the data imediatly when assigned.
     * The pointer must remain valid until the next call of \c activate,
     * e.g. by calling the associated program::draw method of a program.
     *
     * \par Example
     *
     * \code
     * ...
     * \endcode
     *
     * \par Thread Safety
     *
     * - <em>Distinct objects:</em> Safe.
     * - <em>Shared objects:</em> Not safe.
     */
    class attribute_base : public detail::variable<attribute_base>
    {
    public:
        std::size_t size() const { return size_; }

        explicit operator bool() const
        {
            return !desc_.expired();
        }

    protected:
        attribute_base(program& p, std::string name, GLint expected_type, std::size_t dim);

    private:
        friend class globject<attribute_base, with_handle>;
        void create();
        void enable();
        void disable();
        void destroy();

    protected:
        buffer buffer_;
        std::size_t size_;
        bool generic_;

    private:
        std::weak_ptr<detail::attribute_desc> desc_;
        const std::size_t dim_;
    };

    template <typename T, std::size_t Dim>
    class basic_attribute : public attribute_base
    {
        static_assert(1 <= Dim && Dim <= 4, "Dimension must be in range [1,4]");

    public:
        basic_attribute(program& p, std::string name);

        template <std::size_t N>
        basic_attribute& operator=(const T (&rhs)[N]);

        template <std::size_t N>
        basic_attribute& operator=(std::array<T, N> const& rhs);

        template <typename Allocator>
        basic_attribute& operator=(std::vector<T, Allocator> const& rhs);

        void set(T const* const data, std::size_t size);
    };

    template <typename T, std::size_t Dim>
    template <std::size_t N>
    basic_attribute<T, Dim>& basic_attribute<T, Dim>::operator=(const T (&rhs)[N])
    {
        set(rhs, N);
        return *this;
    }

    template <typename T, std::size_t Dim>
    template <std::size_t N>
    basic_attribute<T, Dim>& basic_attribute<T, Dim>::operator=(std::array<T, N> const& rhs)
    {
        set(rhs.data(), rhs.size());
        return *this;
    }

    template <typename T, std::size_t Dim>
    template <typename Allocator>
    basic_attribute<T, Dim>& basic_attribute<T, Dim>::operator=(std::vector<T, Allocator> const& rhs)
    {
        set(rhs.data(), rhs.size());
        return *this;
    }

    template <typename T, std::size_t Dim>
    void basic_attribute<T, Dim>::set(T const* const data, std::size_t size)
    {
        size_ = size / Dim;
        buffer_.set(0, size, data);
    }

    /// Attribute aliases
    namespace attribute
    {
        typedef basic_attribute<float, 1> float_; ///< attribute float
        typedef basic_attribute<float, 2> vec2;   ///< attribute vec2
        typedef basic_attribute<float, 3> vec3;   ///< attribute vec3
        typedef basic_attribute<float, 4> vec4;   ///< attribute vec4

    } // namespace attribute
} // namespace gloo
} // namespace cube
