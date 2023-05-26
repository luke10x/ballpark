#pragma once

# Maybe I don't need this file at all?

#ifdef __EMSCRIPTEN__
#include <GL/glew.h>
#else
#ifdef __APPLE__
#include <GL/glew.h>
#else
#include <GL/glew.h>
#endif
#endif
