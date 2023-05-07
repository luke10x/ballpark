#pragma once

#include<GLFW/glfw3.h>

typedef struct {
  GLuint ID;
} shader_t;

shader_t* shader_create(const char* vertex_file, const char* fragment_file);

void shader_activate(const shader_t* self);

void shader_delete(const shader_t* self);
