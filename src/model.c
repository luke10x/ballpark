// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "ppm.h"
#include "objloader.h"

#define MAX_MESH_COUNT_PER_MODEL 50

void _model_load_meshes(
  model_t* model,
  char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
) {
  mesh_t* mesh = mesh_create(
    name,
    vertices, vertex_count,
    indices, index_count,
    textures, texture_count
  );
  model->meshes[model->mesh_count] = mesh;
  model->mesh_count++;
}

model_t* model_create(char* name) {
  model_t* self = malloc(sizeof(model_t));

  // set up self before passing to loader
  self->meshes = malloc(sizeof(mesh_t) * MAX_MESH_COUNT_PER_MODEL);
  self->mesh_count = 0;

  // TODO move away this outside of this constuructor,
  // so that is can be shared with physics body constructor
  objloader_t* loader = objloader_create(name);
  objloader_register_o_listener(
    loader,
    (void*)self,
    (on_o_load_func*)(&_model_load_meshes)
  );
  objloader_load(loader);

  return self;
}


void model_draw(model_t* model, shader_t* shader, camera_t* camera) {
  for (int i = 0; i < model->mesh_count; i++) {
    mesh_t* mesh = model->meshes[i];
    mesh_draw(mesh, shader, camera );
  }
}
