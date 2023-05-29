#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "ppm.h"
#include "objloader.h"

#define MAX_MATERIALS 256
#define MAX_OBJECTS 256
#define MAX_FACES 4096
#define MAX_VERTICES 4096

typedef struct { float x, y, z; } obj_vertex_t;
typedef struct { float x, y, z; } obj_normal_t;
typedef struct { float u, v; }    t_vt; // "texture vertices"

typedef struct obj_object_t obj_object_t;
struct obj_object_t {
  char* name;
};

objloader_t* objloader_create(char* filename) {

  objloader_t* self =  malloc(sizeof(objloader_t));

  // store filename in struct
  self->filename = malloc((strlen(filename) + 1) * sizeof(char));
  strcpy(self->filename, filename);
  
  // init other fields in struct
  self->o_listener_count = 0;
  self->o_listeners = malloc(sizeof(void*)           * MAX_O_LISTENERS);
  self->on_o_load   = malloc(sizeof(on_o_load_func*) * MAX_O_LISTENERS);

  return self;
}

void objloader_register_o_listener(
  objloader_t* self,
  void* o_listener,
  on_o_load_func* on_o_load
) {
  self->o_listeners[self->o_listener_count] = o_listener;
  self->on_o_load[self->o_listener_count]   = on_o_load;
  self->o_listener_count++;
}

void objloader_load(objloader_t* self) {
  char fullname[30];
  sprintf(fullname, "assets/obj/%s.obj", self->filename);

  FILE *file = fopen(fullname, "r");
  if (!file) {
    printf("Error: Failed to open file %s\n", self->filename);
    return;
  }

  // To hold data parsed from Wavefront OBJ file
  ppm_t*         M = malloc(MAX_MATERIALS * sizeof(ppm_t));
  obj_object_t*  O = malloc(MAX_OBJECTS   * sizeof(obj_object_t));
  obj_vertex_t*  V = malloc(MAX_VERTICES  * sizeof(obj_vertex_t));
  t_vt*         VT = malloc(MAX_VERTICES  * sizeof(t_vt));
  obj_normal_t* VN = malloc(MAX_VERTICES  * sizeof(obj_normal_t));

  // To hold buffers required to create mesh
  GLuint*        I = malloc(MAX_FACES     * sizeof(GLuint) * 3);
  vertex_t*      B = malloc(MAX_VERTICES  * sizeof(vertex_t)); // the real one

  // Set initial counts
  // I don't like that some of them start from -1, some from 0
  // TODO either make them all start with same, or write 
  // a very detailed comment as it allways bothers me
  int v_count  = 0;
  int vt_count = 0;
  int vn_count = 0;
  int o_count  = -1;
  int m_count  = -1;
  int b_count  = 0;

  int offset =0; 
  int tex_idx = 0;

  texture_t** all_textures = malloc(sizeof(texture_t) * 4);

  while (!feof(file)) {
      char line[128];
      if (!fgets(line, 128, file)) break;

      // Vertices (Not our vertices, OBJ vertices, of course)
      if (line[0] == 'v' && line[1] == ' ') {
          sscanf(line, "v %f %f %f",
            &(V[v_count].x), &(V[v_count].y), &(V[v_count].z));
          v_count++;
      }
      
      // Texture vertices
      else if (line[0] == 'v' && line[1] == 't') {
        sscanf(line, "vt %f %f", &(VT[vt_count].u), &(VT[vt_count].v));

        vt_count++;
      }

      // Normals
      else if (line[0] == 'v' && line[1] == 'n') {
        sscanf(line, "vn %f %f %f", &(VN[vn_count].x), &(VN[vn_count].y), &(VN[vn_count].z));
        vn_count++;
      }

      // Objects
      else if (line[0] == 'o' && line[1] == ' ') {

        // If we start an object, and there is an object already
        // then we need to create a mesh first
        if (offset < b_count) {

          vertex_t* vertices = &(B[offset]);
          GLsizei vertex_count = b_count - offset;
          GLuint* indices =  &(I[offset]);
          GLsizei index_count = b_count - offset;
          // notifies all registered listeners
          for (int i = 0; i < self->o_listener_count; i++) {

            void* object_loaded_listener = self->o_listeners[i];
            on_o_load_func on_o_load = (on_o_load_func)self->on_o_load[i];
            on_o_load(
              object_loaded_listener,
              O[o_count].name,
              vertices, vertex_count,
              indices,  index_count,
              all_textures, tex_idx
            );
          }

          offset = b_count;
        }

        o_count++;
        tex_idx = 0;

        // Extract name from second character of the line
        int nameLength = strlen(line) - 2;
        char* obj_name = strdup(line + 2); // Skip "o " prefix
        obj_name[nameLength - 1] = '\0';

        O[o_count].name = obj_name;
      }

      // Materials (starting with "usemtl " prefix)
      else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e') {
        m_count++; 
        
         // Skip "usemtl " prefix
        int nameLength = strlen(line) - 7;
        char* filename = strndup(line + 7,  strlen(line + 7) - 1);

        // TODO Optimize, do not load pixes of already loaded files

        GLuint slt = GL_TEXTURE0 ;
        if (tex_idx == 1) { slt = GL_TEXTURE1; }
        if (tex_idx == 2) { slt = GL_TEXTURE2; }
        if (tex_idx == 3) { slt = GL_TEXTURE3; }
        if (tex_idx > 3) {
          fprintf(stderr, "More then 4 textures detexted!");
          exit(1);
        }
    
        all_textures[tex_idx] = texture_create(
          filename,
          GL_TEXTURE_2D, slt, GL_RGB, GL_UNSIGNED_BYTE
        );

        tex_idx++;
      }

      // Faces 
      else if (line[0] == 'f' && line[1] == ' ') {
        int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
        sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
          &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

        // vertex 1
        B[b_count] = (vertex_t){
          .position = { V[v1 - 1].x, V[v1 - 1].y,  V[v1 - 1].z },
          .color    = { 0.0f, 0.0f, 0.0f },
          .texUV    = { VT[vt1 - 1].u, VT[vt1 - 1].v },
          .tex_id   = tex_idx - 1,
          .normal   = { VN[vn1 - 1].x, VN[vn1 - 1].y, VN[vn1 - 1].z }
        };
        I[b_count] = b_count - offset;
        b_count++;

        // vertex 2
        B[b_count] = (vertex_t){
          .position = { V[v2 - 1].x, V[v2 - 1].y,  V[v2 - 1].z },
          .color    = { 0.0f, 0.0f, 0.0f },
          .texUV    = { VT[vt2 - 1].u, VT[vt2 - 1].v },
          .tex_id   = tex_idx - 1,
          .normal   = { VN[vn2 - 1].x, VN[vn2 - 1].y, VN[vn2 - 1].z }
        };
        I[b_count] = b_count - offset;
        b_count++;

        // vertex 3
        B[b_count] = (vertex_t){
          .position = { V[v3 - 1].x, V[v3 - 1].y,  V[v3 - 1].z },
          .color    = { 0.0f, 0.0f, 0.0f },
          .texUV    = { VT[vt3 - 1].u, VT[vt3 - 1].v },
          .tex_id   = tex_idx - 1 ,
          .normal = { VN[vn3 - 1].x, VN[vn3 - 1].y, VN[vn3 - 1].z }
        };
        I[b_count] = b_count - offset;
        b_count++;
      }
  }
  fclose(file);

  // Last mesh in the file
  vertex_t* vertices = &(B[offset]);
  GLsizei vertex_count = b_count - offset;
  GLuint* indices =  &(I[offset]);
  GLsizei index_count = b_count - offset;

  // notifies all registered listeners
  for (int i = 0; i < self->o_listener_count; i++) {
    void* object_loaded_listener = self->o_listeners[i];
    on_o_load_func on_o_load = (on_o_load_func)self->on_o_load[i];
    on_o_load(
      object_loaded_listener,
      O[o_count].name,
      vertices, vertex_count,
      indices,  index_count,
      all_textures, tex_idx
    );
  }

  free(M);
  free(O);
  free(V);
  free(VT);
  free(VN);

  // printf("Stats\n V=%d\n VT=%d\n VN=%d\n O=%d\n M=%d\n B=%d\n",
  //     v_count, vt_count, vn_count, o_count, m_count, b_count);
}




