#pragma once

#include <stdio.h>
#include <stdlib.h>


#include "gl.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct {
  GLuint ID;
  GLenum type;
  GLuint unit;
} texture_t;

#include "shader.h"

texture_t* texture_create(const char* image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
void texture_unit(texture_t* self, shader_t* shader, const char* uniform, GLuint unit);
void texture_bind(texture_t* self);
void texture_unbind(texture_t* self);
void texture_delete(texture_t* self);
