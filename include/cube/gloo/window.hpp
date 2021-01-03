//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <string>

struct GLFWwindow;

namespace cube
{
namespace gloo
{
    /// Window with OpenGL context.
    /**
     *
     * \par Example
     *
     * \code
     * cube::gloo::window::size_type s = { 400, 400 };
     * std::unique_ptr<cube::gloo::window::strategy> strategy(new shape_strategy);
     * cube::gloo::window win(std::move(strategy), "Display Shape", s);
     * \endcode
     *
     * \par Thread safety
     *
     * - <em>Distinct objects:</em> Safe.
     * - <em>Shared objects:</em> Not safe.
     */
    class window
    {
    public:
        typedef std::array<unsigned, 2> size_type;
        typedef std::array<int, 2> position_type;

        class strategy
        {
            friend class window;

        public:
            strategy();
            virtual ~strategy();

            enum class command
            {
                none,
                close
            };

        private:
            virtual void initialize() = 0;
            virtual void display() = 0;
            virtual void reshape(int w, int h);
            virtual command keyboard(int key) = 0;
            virtual void close();
        };

        window(std::unique_ptr<strategy> stategy, std::string name, size_type size);
        ~window();

        void run();

    private:
        static strategy* const self(GLFWwindow* window);

        void display();
        static void reshape(GLFWwindow* window, int w, int h);
        static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void close(GLFWwindow* window);

    private:
        std::unique_ptr<strategy> strategy_;
        std::string name_;
        size_type size_;
        position_type position_;
        GLFWwindow* window_;
    };
} // namespace gloo
} // namespace cube
