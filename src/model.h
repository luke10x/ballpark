#pragma once

#include "mesh.h"

typedef struct {
  char* name;
  mesh_t** meshes;
  GLsizei mesh_count;
} model_t;

model_t* model_create(char* name);

void model_load_mesh(
  model_t* model,
  char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
);

void model_draw(model_t* model, shader_t* shader, camera_t* camera);