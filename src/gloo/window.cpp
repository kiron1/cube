//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>
#include <iostream>

// clang-format off
#include <cube/gloo/opengl.hpp>
#include <GLFW/glfw3.h>
// clang-format on

#include <cube/gloo/window.hpp>

namespace
{
void error_callback(int error, const char* description)
{
    std::cerr << "Error(" << error << "): " << description << std::endl;
}
} // namespace

namespace cube
{
namespace gloo
{
    window::window(std::unique_ptr<strategy> stategy, std::string name, size_type size)
        : strategy_(std::move(stategy))
        , name_(std::move(name))
        , size_(size)
        , window_(nullptr)
    {
    }

    window::~window()
    {
    }

    window::strategy::strategy()
    {
    }

    window::strategy::~strategy()
    {
    }

    void window::strategy::reshape(int w, int h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
        assert(glGetError() == GL_NO_ERROR);
    }

    window::strategy::command window::strategy::keyboard(int key)
    {
        return window::strategy::command::none;
    }

    void window::strategy::close()
    {
    }

    void window::run()
    {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
        {
            return;
        }

        // Create a windowed mode window and its OpenGL context
        window_ = glfwCreateWindow(size_[0], size_[1], name_.c_str(), NULL, NULL);
        if (!window_)
        {
            glfwTerminate();
            return;
        }

        glfwSetWindowUserPointer(window_, strategy_.get());
        glfwSetKeyCallback(window_, window::keyboard);
        //glfwSetWindowSizeCallback(window_, window::reshape);
        glfwSetFramebufferSizeCallback(window_, window::reshape);
        glfwSetWindowCloseCallback(window_, window::close);

        // make context current, must happen before the call to glewInit
        glfwMakeContextCurrent(window_);

        // after glfwMakeContextCurrent
        const GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            glfwTerminate();
            return;
        }

        strategy_->initialize();
        assert(glGetError() == GL_NO_ERROR);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window_))
        {
            // Render here
            display();

            glfwSwapBuffers(window_);

            glfwPollEvents();
        }

        glfwTerminate();
    }

    window::strategy* const window::self(GLFWwindow* window)
    {
        void* const data = glfwGetWindowUserPointer(window);
        assert(data != nullptr);
        return static_cast<strategy*>(data);
    }

    void window::display()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        assert(glGetError() == GL_NO_ERROR);

        strategy_->display();
        assert(glGetError() == GL_NO_ERROR);
    }

    void window::reshape(GLFWwindow* window, int w, int h)
    {
        self(window)->reshape(w, h);
    }

    void window::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        const auto cmd = self(window)->keyboard(key);
        switch (cmd)
        {
        case strategy::command::none:
            break;
        case strategy::command::close:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        }
    }

    void window::close(GLFWwindow* window)
    {
        self(window)->close();
    }
} // namespace gloo
} // namespace cube
