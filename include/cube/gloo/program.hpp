//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <cube/gloo/opengl.hpp>

#include <cube/gloo/element_index.hpp>
#include <cube/gloo/globject.hpp>
#include <cube/gloo/shader.hpp>
#include <cube/gloo/variable.hpp>

namespace cube
{
namespace gloo
{
    namespace detail
    {
        struct variable_desc
        {
            /*const*/ GLint location;
            /*const*/ GLint size;
            /*const*/ GLenum type;
        };

        struct attribute_desc : variable_desc
        {
        };

        struct uniform_desc : variable_desc
        {
        };
    }

    /// OpenGL shader program, linking the \ref shaders and binding the \ref variables.
    /**
     * \anchor program
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
    class program : public globject<program, with_handle>
    {
        friend class attribute_base;
        friend class uniform_base;

    public:
        typedef std::unordered_map<std::string, std::shared_ptr<detail::attribute_desc>> attribute_table;
        typedef std::unordered_map<std::string, std::shared_ptr<detail::uniform_desc>> uniform_table;

        enum draw_mode
        {
            points = GL_POINTS,
            lines = GL_LINES,
            line_strip = GL_LINE_STRIP,
            line_loop = GL_LINE_LOOP,
            triangles = GL_TRIANGLES,
            triangle_strip = GL_TRIANGLE_STRIP,
            triangle_fan = GL_TRIANGLE_FAN
        };

        program();
        program(vertex_shader vertex, fragment_shader fragment);
        program(std::string vertex, std::string fragment);

        /// Attach a vertex shader to the program.
        void attach(vertex_shader const& shader);

        /// Attach a fragment shader to the program.
        void attach(fragment_shader const& shader);

        /// Draw
        void draw(draw_mode mode);

        /// Draw using a \ref element_index.
        void draw(draw_mode mode, element_index_base& indices);

    private:
        void attach(attribute_base* symbol);
        void attach(uniform_base* symbol);

        std::weak_ptr<detail::attribute_desc> variable(attribute_base* symbol);
        std::weak_ptr<detail::uniform_desc> variable(uniform_base* symbol);

        void detach(attribute_base* desc);
        void detach(uniform_base* desc);

        attribute_table active_attributes();
        uniform_table active_uniforms();

    private:
        friend class globject<program, with_handle>;
        void create();
        void enable();
        void disable();
        void destroy();

    private:
        std::vector<vertex_shader> vertex_shaders_;
        std::vector<fragment_shader> fragment_shaders_;

        attribute_table attribute_locations_;
        uniform_table uniform_locations_;

        std::unordered_set<attribute_base*> attached_attributes_;
        std::unordered_set<uniform_base*> attached_uniforms_;

        bool need_link_;
    };
}
}
