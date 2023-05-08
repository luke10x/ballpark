#pragma once

#include <string.h>

#include "buffers.h"
#include "camera.h"
#include "texture.h"
#pragma once

typedef struct {
  vertex_t* vertices;
  GLsizei vertex_count;

  GLuint* indices;
  GLsizei index_count;

  texture_t* textures;
  GLsizei texture_count;

  vao_t* vao;
} mesh_t;


mesh_t* mesh_create(
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t* textures,
  GLsizei texture_count
);

void mesh_draw(mesh_t* mesh, shader_t* shader, camera_t* camera);
