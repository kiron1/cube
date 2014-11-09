//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <array>
#include <chrono>
#include <string>
#include <memory>

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

        protected:
            /// Leave main loop.
            static void leave();

            /// Retrieve elapsed time in msec.
            static std::chrono::milliseconds time_elapsed();

            /// Start periodic timer
            /**
              * \param time Period in msec.
              */
            static void periodic(int time);

        private:
            virtual void initialize() = 0;
            virtual void display() = 0;
            virtual void reshape(int w, int h);
            virtual void keyboard(unsigned char key, int w, int h);
            virtual void idle();
            virtual void timer();
            virtual void close();
        };

        window(std::unique_ptr<strategy> stategy, std::string name, size_type size);
        ~window();

        void run();

    private:
        static strategy* const self();

        static void display();
        static void reshape(int w, int h);
        static void keyboard(unsigned char, int w, int h);
        static void idle();
        static void timer(int value);
        static void close();

    private:
        std::unique_ptr<strategy> stategy_;
        std::string name_;
        size_type size_;
        position_type position_;
        int window_;
    };
}
}
