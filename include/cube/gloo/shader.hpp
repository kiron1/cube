//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <string>

#include <cube/gloo/opengl.hpp>

#include <cube/gloo/globject.hpp>

/**
 * \addtogroup shader OpenGL shader
 * \{
 * \anchor shaders
 *
 * OpenGL shaders.
 */

namespace cube
{
namespace gloo
{
    namespace detail
    {
        class shader : public globject<shader, with_handle>
        {
        public:
            void code(std::string source);

            bool compiled() const { return compiled_; }

        protected:
            explicit shader(GLuint type);

        private:
            friend class globject<shader, with_handle>;
            void create();
            void enable();
            void disable();
            void destroy();

        protected:
            const GLuint type_;

        private:
            std::string source_;
            bool compiled_;
        };
    }

    /// Vertex shader linked to a \ref program.
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
    class vertex_shader : public detail::shader
    {
    public:
        /// construct an empty shader.
        vertex_shader();

        /// Construct a shader from source.
        explicit vertex_shader(std::string src);
    };

    /// Fragment shader linked to a \ref program.
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
    class fragment_shader : public detail::shader
    {
    public:
        /// Construct an empty shader.
        fragment_shader();

        /// Construct a shader from source.
        explicit fragment_shader(std::string src);
    };
}
}

/** \} */
