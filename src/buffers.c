#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include<GLFW/glfw3.h>
#endif

#ifdef __EMSCRIPTEN__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#endif

/*
 * An Element Buffer Object (EBO) is a buffer
 * that stores indices for indexed rendering.
 * Instead of duplicating vertex data for each triangle,
 * a list of indices is used to specify which vertices to use.
 * This can save memory and improve performance in certain cases.
 */
typedef struct {
  GLuint ID;
} ebo_t;

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

/*
 * A Vertex Buffer Object (VBO) is a buffer
 * that stores vertex data such as vertex positions,
 * normals, texture coordinates, and other per-vertex data.
 * VBOs are used to transfer large amounts of vertex data
 * to the GPU efficiently.
 */
typedef struct {
  GLuint ID;
} vbo_t;

vbo_t* vbo_create(GLfloat* vertices, GLsizeiptr size) {
  vbo_t* self = malloc(sizeof(vbo_t));
  glGenBuffers(1, &(self->ID));
  glBindBuffer(GL_ARRAY_BUFFER, self->ID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);  
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

/*
 * A Vertex Array Object (VAO) is an object
 * that encapsulates all the state needed to specify
 * how to access vertex data in a buffer object.
 * It specifies the format of the vertex data
 * and how to read it from the Vertex Buffer Objects (VBOs).
 */
typedef struct {
  GLuint ID;
} vao_t;

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