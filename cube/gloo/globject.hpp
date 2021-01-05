//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include "cube/gloo/opengl.hpp"

namespace cube
{
namespace gloo
{
    /// Policy to add a handle to a \ref globject.
    class with_handle
    {
    public:
        with_handle()
            : handle_(0)
        {
        }
        GLuint handle() const { return handle_; }

    protected:
        void handle(GLuint u) { handle_ = u; }

    private:
        GLuint handle_;
    };

    /// Policy to add a target to a \ref globject.
    class with_target
    {
    public:
        with_target()
            : target_(0)
        {
        }
        GLuint target() const { return target_; }

    protected:
        void target(GLuint u) { target_ = u; }

    private:
        GLuint target_;
    };

    /// CRTP base class which handles the state of an OpenGL object.
    /**
     *
     * When the OpenGL object is activated by calling \ref activate
     * it will call \c create and then \c enable of its derived class.
     * If the OpenGL object was created by a previous \ref activate call
     * only the \c enable method of the derived class will be called.
     *
     * Calling \ref deactivate, will call the \c disable method of the
     * derived class if allowed.
     *
     * The \ref dispose method will call \c destroy of the derived class.
     * Inside the \c destroy method, the resources acquired in \c create
     * must be cleaned up.
     *
     * The methods \ref activate, \ref deactivate and \ref dispose must be
     * called inside a valid OpenGL context.
     *
     * In general, public methods of a derived class from globject shall
     * not require a valid opengl context. Public methods can be provided
     * which store the requested modification to apply on the OpenGL object.
     * When \ref activate is called these modifications can be executed
     * inside a valid OpenGL context by implementing the \c enable method
     * in the derived class.
     *
     * \par Example:
     * \code
     * class buffer : public globject<buffer, with_handle, with_target>
     * {
     * private:
     *     friend class globject<buffer, with_handle, with_target;
     *     void create();
     *     void enable();
     *     void disable();
     *     void destroy();
     * };
     * \endcode
     */
    template <typename Derived, typename... Policies>
    class globject : public Policies...
    {
    public:
        enum state_type
        {
            constructed,
            idle,
            enabled,
            dead
        };

        state_type state() const { return state_; }

        /// Activate this OpenGL object (will create (if necessary) and
        /// enable the
        /// object), must be called inside a valid OpenGL context.
        void activate()
        {
            switch (state_)
            {
            case constructed:
                static_cast<Derived*>(this)->create();
            // no break
            case idle:
                static_cast<Derived*>(this)->enable();
                state_ = enabled;
                break;
            case enabled:
            case dead:
                break;
            }
        }

        /// Deactivate this OpenGL object (disables the object), must be
        /// called
        /// inside a valid OpenGL context.
        void deactivate()
        {
            switch (state_)
            {
            case enabled:
                static_cast<Derived*>(this)->disable();
                state_ = idle;
                break;
            case constructed:
            case idle:
            case dead:
                break;
            }
        }

        /// Dispose this OpenGL object (delete its resources), must be
        /// called inside
        /// a valid OpenGL context.
        void dispose()
        {
            switch (state_)
            {
            case constructed:
                // not constructed, no need to destroy
                break;
            case enabled:
                static_cast<Derived*>(this)->disable();
            // no break
            case idle:
                static_cast<Derived*>(this)->destroy();
                state_ = dead;
                break;
            case dead:
                break;
            }
        }

    protected:
        globject()
            : state_(constructed)
        {
        }

    private:
        // globject(globject const& other);

        // globject& operator =(globject const& other);
    private:
        state_type state_;
    };

    /// Activate a range of OpenGL objects, calls for each item \ref
    /// globject::activate().
    template <typename Iterator>
    void activate_all(Iterator first, Iterator const& last)
    {
        for (; first != last; ++first)
        {
            first->activate();
        }
    }

    /// Deactivate a range of OpenGL objects, calls for each item
    /// globject::deactivate().
    template <typename Iterator>
    void deactivate_all(Iterator first, Iterator const& last)
    {
        for (; first != last; ++first)
        {
            first->deactivate();
        }
    }

    /// Dispose a range of OpenGL objects, calls for each item
    /// globject::dispose().
    template <typename Iterator>
    void dispose_all(Iterator first, Iterator const& last)
    {
        for (; first != last; ++first)
        {
            first->dispose();
        }
    }
} // namespace gloo
} // namespace cube
