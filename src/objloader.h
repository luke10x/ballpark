#pragma once

#define MAX_O_LISTENERS 4

typedef void (*on_o_load_func)(
  void* o_listener,
  char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
);

typedef struct {
  char* filename;

  // registereed listeners
  void** o_listeners;
  on_o_load_func** on_o_load;
  int o_listener_count;

} objloader_t;

objloader_t* objloader_create(char *filename);

void objloader_register_o_listener(
  objloader_t* self, // loader
  void* o_listener,
  on_o_load_func* on_o_load
);

void objloader_load(objloader_t* self);