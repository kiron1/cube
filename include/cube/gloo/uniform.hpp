//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <cassert>
#include <algorithm>
#include <string>
#include <array>
#include <vector>
#include <memory>

#include <cube/gloo/detail/variable.hpp>
#include <cube/gloo/globject.hpp>
#include <cube/gloo/buffer.hpp>

namespace cube
{
namespace gloo
{
    class program;

    class uniform_base : public detail::variable<uniform_base>
    {
    public:
        explicit operator bool() const
        {
            return !desc_.expired();
        }

    protected:
        uniform_base(program& p, std::string name, GLint expected_type);

    private:
        virtual void transfer() const = 0;

    private:
        friend class globject<uniform_base, with_handle>;
        void create();
        void enable();
        void disable();
        void destroy();

    protected:
        bool dirty_;

    private:
        std::weak_ptr<detail::uniform_desc> desc_;
    };

    /// Uniform vector proxy of an OpenGL program.
    /**
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
    template <typename T, std::size_t N>
    class basic_uniform_vector : public uniform_base
    {
    public:
        /// Construct a uniform vector proxy.
        /**
         * \param p OpenGL program.
         * \param name Name of the unifrom in the shader.
         */
        basic_uniform_vector(program& p, std::string name);

        /// Assign a value to the uniform, \sa basic_uniform_vector::set.
        basic_uniform_vector& operator=(const T (&rhs)[N]);

        /// Assign a value to the uniform, \sa basic_uniform_vector::set.
        basic_uniform_vector& operator=(std::array<T, N> const& rhs);

        /// Assign a value to the uniform, \sa basic_uniform_vector::set.
        template <typename Allocator>
        basic_uniform_vector& operator=(std::vector<T, Allocator> const& rhs);

        /// Raw interface to set the data of the uniform.
        void set(T const* const data);

    private:
        void transfer() const;

    private:
        T data_[N];
    };

    /// Unifrom matrix proxy of a OpenGL program.
    /**
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
    template <typename T, std::size_t N>
    class basic_uniform_matrix : public uniform_base
    {
    public:
        typedef T value_type[N * N];

        /// Construct a uniform matrix proxy.
        /**
         * \param p OpenGL program.
         * \param name Name of the unifrom in the shader.
         */
        basic_uniform_matrix(program& p, std::string name);

        /// Assign a value to the uniform, \sa basic_uniform_matrix::set.
        basic_uniform_matrix& operator=(const T (&rhs)[N * N]);

        /// Assign a value to the uniform, \sa basic_uniform_matrix::set.
        basic_uniform_matrix& operator=(std::array<T, N * N> const& rhs);

        /// Assign a value to the uniform, \sa basic_uniform_matrix::set.
        template <typename Allocator>
        basic_uniform_matrix& operator=(std::vector<T, Allocator> const& rhs);

        /// Raw interface to set the data of the uniform.
        void set(T const* const data);

        value_type const& data() const { return data_; }

        value_type& data() { return data_; }

    private:
        void transfer() const;

    private:
        value_type data_;
    };

    /// Uniform aliases.
    namespace uniform
    {
        typedef basic_uniform_vector<float, 1> float_; ///< uniform float
        typedef basic_uniform_vector<float, 2> vec2;   ///< uniform vec2
        typedef basic_uniform_vector<float, 3> vec3;   ///< uniform vec3
        typedef basic_uniform_vector<float, 4> vec4;   ///< uniform vec4
        typedef basic_uniform_matrix<float, 2> mat2;   ///< uniform mat2
        typedef basic_uniform_matrix<float, 3> mat3;   ///< uniform mat3
        typedef basic_uniform_matrix<float, 4> mat4;   ///< uniform mat4
    }

    template <typename Derived>
    detail::variable<Derived>::variable(program& p, std::string name, GLint expected_type)
        : program_(&p)
        , name_(std::move(name))
        , expected_type_(expected_type)
    {
    }

    template <typename T, std::size_t N>
    basic_uniform_vector<T, N>& basic_uniform_vector<T, N>::operator=(const T (&rhs)[N])
    {
        set(rhs);
        return *this;
    }

    template <typename T, std::size_t N>
    basic_uniform_vector<T, N>& basic_uniform_vector<T, N>::operator=(std::array<T, N> const& rhs)
    {
        set(rhs.data());
        return *this;
    }

    template <typename T, std::size_t N>
    template <typename Allocator>
    basic_uniform_vector<T, N>& basic_uniform_vector<T, N>::operator=(std::vector<T, Allocator> const& rhs)
    {
        assert(rhs.size() == N);
        set(rhs.data());
        return *this;
    }

    template <typename T, std::size_t N>
    void basic_uniform_vector<T, N>::set(T const* const data)
    {
        std::copy(data, data + N, data_);
        dirty_ = true;
    }

    template <typename T, std::size_t N>
    basic_uniform_matrix<T, N>& basic_uniform_matrix<T, N>::operator=(const T (&rhs)[N * N])
    {
        set(rhs);
        return *this;
    }

    template <typename T, std::size_t N>
    basic_uniform_matrix<T, N>& basic_uniform_matrix<T, N>::operator=(std::array<T, N * N> const& rhs)
    {
        set(rhs.data());
        return *this;
    }

    template <typename T, std::size_t N>
    template <typename Allocator>
    basic_uniform_matrix<T, N>& basic_uniform_matrix<T, N>::operator=(std::vector<T, Allocator> const& rhs)
    {
        assert(rhs.size() == N * N);
        set(rhs.data());
        return *this;
    }

    template <typename T, std::size_t N>
    void basic_uniform_matrix<T, N>::set(T const* const data)
    {
        std::copy(data, data + N * N, data_);
        dirty_ = true;
    }

    /*
    template <typename T, std::size_t N>
    void eye(basic_uniform_matrix<T, N>& m)
    {
        for(std::size_t i = 0; i != N; ++i)
        {
            for(std::size_t k = 0; k != N; ++k)
            {
                std::size_t const idx = i * N + k;
                data_[idx] = (i == k) ? 1 : 0;
            }
        }
    }

    template <typename T, std::size_t N>
    void mtimes()
    {
        for(std::size_t i = 0; i != N; ++i)
        {
            for(std::size_t k = 0; k != N; ++k)
            {
                std::size_t const idx = i * N + k;
                data_[idx] = (i == k) ? 1 : 0;
            }
        }
    }
    */
}
}
