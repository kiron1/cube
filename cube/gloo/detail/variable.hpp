//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#pragma once

#include <string>

#include <cube/gloo/globject.hpp>

namespace cube
{
namespace gloo
{
    class program;

    namespace detail
    {
        struct attribute_desc;
        struct uniform_desc;

        template <typename Derived>
        class variable : public globject<Derived, with_handle>
        {
        public:
            std::string const& name() const { return name_; }

            GLint expected_type() const { return expected_type_; }

        protected:
            variable(program& p, std::string name, GLint expected_type);

        protected:
            program* program_;

        private:
            std::string name_;
            const GLint expected_type_;
        };
    } // namespace detail
} // namespace gloo
} // namespace cube
