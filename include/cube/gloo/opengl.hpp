//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

// Extension point to hook in a different OpenGL header for mocking.
// clang-format off
#ifndef CUBE_GLOO_GL_H
#define CUBE_GLOO_GL_H "GL/glew.h"
#endif
// clang-format on

#include CUBE_GLOO_GL_H
