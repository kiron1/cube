//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <cassert>

#include <cube/gloo/opengl.hpp>
#include <GL/freeglut.h>

#include <cube/gloo/window.hpp>

namespace cube
{
namespace gloo
{
    window::window(std::unique_ptr<strategy> stategy, std::string name, size_type size)
        : stategy_(std::move(stategy))
        , name_(std::move(name))
        , size_(size)
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

    void window::strategy::leave()
    {
        glutLeaveMainLoop();
    }

    std::chrono::milliseconds window::strategy::time_elapsed()
    {
        std::chrono::milliseconds const ms{glutGet(GLUT_ELAPSED_TIME)};
        return ms;
    }

    void window::strategy::periodic(int time)
    {
        glutTimerFunc(time, window::timer, 0);
    }

    void window::strategy::reshape(int w, int h)
    {
        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
        assert(glGetError() == GL_NO_ERROR);
        glutPostRedisplay();
    }

    void window::strategy::keyboard(unsigned char key, int w, int h)
    {
    }

    void window::strategy::idle()
    {
    }

    void window::strategy::timer()
    {
    }

    void window::strategy::close()
    {
    }

    void window::run()
    {
        int argc = 0;
        glutInit(&argc, 0);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

        glutInitWindowSize(size_[0], size_[1]);

        window_ = glutCreateWindow(name_.c_str());
        GLenum err = glewInit();
        assert(err == GLEW_OK);

        glutSetWindowData(stategy_.get());

        glutDisplayFunc(window::display);
        glutReshapeFunc(window::reshape);
        glutKeyboardFunc(window::keyboard);
        glutIdleFunc(window::idle);
        glutCloseFunc(window::close);

        glutReshapeWindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

        self()->initialize();
        assert(glGetError() == GL_NO_ERROR);

        glutMainLoop();
    }

    window::strategy* const window::self()
    {
        void* const data = glutGetWindowData();
        assert(data != nullptr);
        return static_cast<strategy*>(data);
    }

    void window::display()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        assert(glGetError() == GL_NO_ERROR);

        self()->display();
        assert(glGetError() == GL_NO_ERROR);

        glutSwapBuffers();
    }

    void window::reshape(int w, int h)
    {
        self()->reshape(w, h);
    }

    void window::keyboard(unsigned char key, int w, int h)
    {
        self()->keyboard(key, w, h);
    }

    void window::idle()
    {
        self()->idle();
    }

    void window::timer(int value)
    {
        self()->timer();
    }

    void window::close()
    {
        self()->close();
    }
}
}
