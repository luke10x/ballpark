#include <stdio.h>
#include <stdlib.h>

#include "buffers.h"

ebo_t* ebo_create(GLuint* indices, GLsizeiptr size) {
  ebo_t* self = malloc(sizeof(ebo_t));
  glGenBuffers(1, &(self->ID));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);  
  return self;
}

void ebo_bind  (ebo_t* self) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ID);
}

void ebo_unbind(ebo_t* self) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_delete(ebo_t* self) {
  // glDeleteBuffers(1, self->ID);
  free(self);
}

vbo_t* vbo_create(vertex_t* vertices, GLsizei size) {
  vbo_t* self = malloc(sizeof(vbo_t));
  glGenBuffers(1, &(self->ID));
  glBindBuffer(GL_ARRAY_BUFFER, self->ID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  self->vertice_count = size; 
  return self;
}

void vbo_bind  (vbo_t* self) {
  glBindBuffer(GL_ARRAY_BUFFER, self->ID);
}

void vbo_unbind(vbo_t* self) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(vbo_t* self) {
  // glDeleteBuffers(1, self->ID);
  free(self);
}

vao_t* vao_create() {
  vao_t* self = malloc(sizeof(vao_t));
  glGenVertexArrays(1, &(self->ID));
  return self;
}

void vao_link_attrib(vao_t* self, vbo_t* vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
  vbo_bind(vbo);

  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  vbo_unbind(vbo);
}

void vao_bind  (vao_t* self) {
  glBindVertexArray(self->ID);
}

void vao_unbind(vao_t* self) {
  glBindVertexArray(0);
}