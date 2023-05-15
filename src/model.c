#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "ppm.h"

#define MAX_LINE_SIZE 1024
#define MAX_MATERIALS 128
#define MAX_OBJECTS 128
#define MAX_FACES 2048
#define MAX_VERTICES 2048

typedef struct {
  uint8_t r,g,b;
} t_pixel;

typedef struct {
  float x, y, z;
} t_vertex;

typedef struct {
  float u, v;
} t_vt;

typedef struct t_object t_object;
typedef struct t_face t_face;

struct t_face {
  t_vertex v1, v2, v3;
  t_vt vt1, vt2, vt3;
  t_object* obj;
  ppm_t *m;
};

struct t_object {
  char* name;
};

/**
 * @Returns number of faces
 */
int buffers_load_from_file(const char* filename, mesh_t** buffers) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: Failed to open file %s\n", filename);
    return 0;
  }

  GLuint*     I = malloc(MAX_FACES     *sizeof(GLuint) * 3);
  t_object*   O = malloc(MAX_OBJECTS   *sizeof(t_object));
  ppm_t*      M = malloc(MAX_MATERIALS *sizeof(ppm_t));
  t_vertex*   V = malloc(MAX_VERTICES  *sizeof(t_vertex));
  vertex_t*   B = malloc(MAX_VERTICES  *sizeof(vertex_t)); // the real one
  t_vt*      VT = malloc(MAX_VERTICES  *sizeof(t_vt));
  vec3*      VN = malloc(MAX_VERTICES  *sizeof(vec3));

  int v_count  = 0;
  int vt_count = 0;
  int vn_count = 0;
  int o_count  = -1;
  int m_count  = -1;
  int b_count  = 0;

  int offset =0; 
  int mesh_added = 0;

  texture_t* current_texture = NULL; 

  while (!feof(file)) {
      char line[128];
      if (!fgets(line, 128, file)) break;

      // Vertices
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
        sscanf(line, "vn %f %f %f", &(VN[vn_count][0]), &(VN[vn_count][1]), &(VN[vn_count][2]));
        vn_count++;
      }

      // Objects
      else if (line[0] == 'o' && line[1] == ' ') {

        // If we start an object, and there is an object already
        // then we need to create a mesh first
        if (offset < b_count) {
          mesh_t* new_mesh = mesh_create(
            O[o_count].name,
            &(B[offset]), (b_count - offset) * sizeof(vertex_t),
            &(I[offset]), (b_count - offset) * sizeof(GLuint),
            current_texture, 1
          );

          buffers[mesh_added] = new_mesh;

          mesh_added++;
          offset = b_count;
        }

        o_count++;

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
        current_texture = texture_create(
          filename,
          GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE
        );
      }

      // Finally faces 
      else if (line[0] == 'f' && line[1] == ' ') {
        int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
        sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
          &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

        // vertex 1
        B[b_count] = (vertex_t){
          .position = {V[v1 - 1].x, V[v1 - 1].y,  V[v1 - 1].z},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt1 - 1].u, VT[vt1 - 1].v },
          .normal = { VN[vn1 - 1][0], VN[vn1 - 1][1], VN[vn1 - 1][2]}
        };
        I[b_count] = b_count - offset;
        b_count++;

        // vertex 2
        B[b_count] = (vertex_t){
          .position = {V[v2 - 1].x, V[v2 - 1].y,  V[v2 - 1].z},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt2 - 1].u, VT[vt2 - 1].v },
          .normal = { VN[vn2 - 1][0], VN[vn2 - 1][1], VN[vn2 - 1][2]}
        };
        I[b_count] = b_count - offset;
        b_count++;

        // vertex 3
        B[b_count] = (vertex_t){
          .position = {V[v3 - 1].x, V[v3 - 1].y,  V[v3 - 1].z},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt3 - 1].u, VT[vt3 - 1].v },
          .normal = { VN[vn3 - 1][0], VN[vn3 - 1][1], VN[vn3 - 1][2]
          }
        };
        I[b_count] = b_count - offset;
        b_count++;
      }
  }
  fclose(file);

  // Last mesh in the file
  mesh_t* new_mesh = mesh_create(
    O[o_count].name,
    &(B[offset]), (b_count - offset) * sizeof(vertex_t),
    &(I[offset]), (b_count - offset) * sizeof(GLuint),
    current_texture, 1
  );

  buffers[mesh_added] = new_mesh;
  mesh_added++;

  free(VT);
  free(V);

  return mesh_added;
}

model_t* model_create(char* name) {
  model_t* self = malloc(sizeof(model_t));

  self->meshes = malloc(sizeof(mesh_t) * 50);

  int num_buffers = buffers_load_from_file("assets/obj/cube.obj", self->meshes);
  self->mesh_count = num_buffers;

  printf("Meshes loaded: %d\n", num_buffers);

  return self;
}


void model_draw(model_t* model, shader_t* shader, camera_t* camera) {
  for (int i = 0; i < model->mesh_count; i++) {
    mesh_t* mesh = model->meshes[i];
    mesh_draw(mesh, shader, camera );
  }
}
