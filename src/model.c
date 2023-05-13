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

// typedef struct {
//   char* name;
//   t_pixel* pixels;
//   int width, height;
// } t_material;

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

  t_face*     F = malloc(MAX_FACES     *sizeof(t_face));
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
  int f_count  = 0;

  int offset =0; 
  int mesh_added = 0;
  mesh_t* mesh;

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
        sscanf(line, "vn %f %f %f", &(VN[vt_count][0]), &(VN[vt_count][1]), &(VN[vt_count][2]));
        vn_count++;
      }

      // Objects
      else if (line[0] == 'o' && line[1] == ' ') {


        // If we start an object, and there is an object already
        // then we need to create a mesh first
        if (offset < b_count && mesh_added == 0) {
          mesh_added++;
          vertex_t* old_mesh_start = &(B[offset]);
          GLsizei old_mesh_size = b_count - offset;
          
          printf("Creating mesh with vertices:\n");
          for (int i = offset; i < b_count; i++) {
              vertex_t* vert = &(old_mesh_start[i]);
              printf(" { .position = {%.3f, %.3f,  %.3f},  .normal = {%.3f, %.3f,  %.3f}, .color = { %.3f, %.3f,  %.3f}, .texUV={ %.3f, %.3f } },\n",
                vert->position[0], vert->position[1], vert->position[2],
                vert->normal[0], vert->normal[1], vert->normal[2],
                vert->color[0], vert->color[1], vert->color[2],
                vert->texUV[0], vert->texUV[1]
              );
          }
          printf("Creating mesh with indices: [ \n");
          GLuint* old_mesh_indices_start = &(I[offset]);
          GLsizei old_mesh_indixes_size = b_count - offset; // same
          for (int i = offset; i < b_count; i++) {
            GLuint idx = (old_mesh_indices_start[i]);
            printf("%d, ", idx);
          }
          printf("] //end\n");


            texture_t* pop_cat = texture_create(
              "05-01-wall",
              GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE
            );
          mesh = mesh_create("first", old_mesh_start, old_mesh_size * sizeof(vertex_t),
                            old_mesh_indices_start, old_mesh_indixes_size * sizeof(GLuint),
                            pop_cat, 1);

          offset = b_count;

        }

        o_count++;

        int nameLength = strlen(line) - 2;
        O[o_count].name = strdup(line + 2); // Skip "o " prefix
        O[o_count].name[nameLength - 1] = '\0';
      }

      // Materials (starting with "usemtl " prefix)
      else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e') {
        m_count++; 
        
        int nameLength = strlen(line) - 7;
        char* filename = strndup(line + 7,  strlen(line + 7) - 1); // Skip "usemtl " prefix
        // TODO Optimize, do not load pixes of already loaded files
        // t_material* material = &M[m_count];
        ppm_t* material = &M[m_count];
        ppm_load(filename, &material, 0);

      }

      // Finally faces
      else if (line[0] == 'f' && line[1] == ' ') {
        int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
        sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
          &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
        
        // Create face from scanned data
        F[f_count].v1.x = V[v1 - 1].x;
        F[f_count].v1.y = V[v1 - 1].y;
        F[f_count].v1.z = V[v1 - 1].z;


        B[b_count] = (vertex_t){
          .position = {V[v1 - 1].x, V[v1 - 1].y,  V[v1 - 1].z},
          .normal = { VN[vn1 - 1][0], VN[vn1 - 1][1], VN[vn1 - 1][2]},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt1 - 1].u, VT[vt1 - 1].v }
        };
        I[b_count] = b_count;
        b_count++;

        F[f_count].v2.x = V[v2 - 1].x;
        F[f_count].v2.y = V[v2 - 1].y;
        F[f_count].v2.z = V[v2 - 1].z;
        B[b_count] = (vertex_t){
          .position = {V[v2 - 1].x, V[v2 - 1].y,  V[v2 - 1].z},
          .normal = { VN[vn2 - 1][0], VN[vn2 - 1][1], VN[vn2 - 1][2]},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt2 - 1].u, VT[vt2 - 1].v }
        };
        I[b_count] = b_count;
        b_count++;

        F[f_count].v3.x = V[v3 - 1].x;
        F[f_count].v3.y = V[v3 - 1].y;
        F[f_count].v3.z = V[v3 - 1].z;
        B[b_count] = (vertex_t){
          .position = {V[v3 - 1].x, V[v3- 1].y,  V[v3 - 1].z},
          .normal = { VN[vn3 - 1][0], VN[vn3 - 1][1], VN[vn3 - 1][2]},
          .color = { 0.0f, 0.0f, 0.0f },
          .texUV={ VT[vt3 - 1].u, VT[vt3 - 1].v }
        };
        I[b_count] = b_count;
        b_count++;

        F[f_count].vt1.u = VT[vt1 - 1].u;
        F[f_count].vt1.v = VT[vt1 - 1].v;
        F[f_count].vt2.u = VT[vt2 - 1].u;
        F[f_count].vt2.v = VT[vt2 - 1].v;
        F[f_count].vt3.u = VT[vt3 - 1].u;
        F[f_count].vt3.v = VT[vt3 - 1].v;

        // Current object and material too...
        F[f_count].m   = &M[m_count];
        F[f_count].obj = &O[o_count];

        f_count++;
      }
  }
  fclose(file);

  free(VT);
  free(V);

  // *faces = F;
  *buffers = mesh;
  return b_count;

  // This could be usefull for debug
  for (int i = 0; i < f_count; i++) {
    printf("F: %d/%d in '%s' using '%s' (%dx%d) \n",
    i+1, f_count, F[i].obj->name, F[i].m->name, F[i].m->width, F[i].m->height );
    printf(" - Vertex (%3.3f, %3.3f, %3.3f) with UV (%.3f, %.3f)\n",
      F[i] .v1.x,
      F[i] .v1.y,
      F[i] .v1.z,
      F[i].vt1.u,
      F[i].vt1.v
    );
    printf(" - Vertex (%3.3f, %.3f, %3.3f) with UV (%.3f, %.3f)\n",
      F[i] .v2.x,
      F[i] .v2.y,
      F[i] .v2.z,
      F[i].vt2.u,
      F[i].vt2.v
    );
    printf(" - Vertex (%3.3f, %3.3f, %3.3f) with UV (%.3f, %.3f)\n",
      F[i] .v3.x,
      F[i] .v3.y,
      F[i] .v3.z,
      F[i].vt3.u,
      F[i].vt3.v
    );
  }
  return b_count;
  // return f_count;
}

model_t* model_create(char* name) {

  model_t* self = malloc(sizeof(model_t));


  mesh_t* buffers = NULL;

  int num_buffers = buffers_load_from_file("assets/obj/luke.obj", &buffers);

  self->meshes = buffers;
  self->mesh_count = num_buffers;


  printf("Meshes loaded: %d\n", num_buffers);

  return self;

}


void model_draw(model_t* model, shader_t* shader, camera_t* camera) {
  mesh_t* first_mesh = model->meshes;
  mesh_draw(first_mesh, shader, camera );

}
