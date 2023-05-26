#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/glew.h>
#else
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#endif
