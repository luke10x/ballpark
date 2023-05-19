#include "mesh.h"

mesh_t* mesh_create(
  char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
) {
  if (textures == NULL) {
    printf("texture not set while trying to create mesh for \"%s\"\n", name);
  }

#ifdef DEBUG
  printf("Creating mesh \"%s\" with vertices:\n", name);
  for (int i = 0; i < vertex_count/ sizeof(vertex_t); i++) {
      vertex_t* vert = &(vertices[i]);
      printf(" { .position = {%.3f, %.3f,  %.3f}, .color = { %.3f, %.3f,  %.3f}, .texUV={ %.3f, %.3f }, .normal = {%.3f, %.3f,  %.3f}},\n",
        vert->position[0], vert->position[1], vert->position[2],
        vert->color[0], vert->color[1], vert->color[2],
        vert->texUV[0], vert->texUV[1],
        vert->normal[0], vert->normal[1], vert->normal[2]
      );
  }

  printf("  with indices: [ ");
  for (int i = 0; i < index_count/ sizeof(GLuint); i++) {
    GLuint idx = (indices[i]);
    printf("%d, ", idx);
  }
  printf("];\n");
#endif

  mesh_t* mesh = malloc(sizeof(mesh_t));

  // TODO copy vertices and indices, becuase it is owned by outside code,
  // and can be gone at any moment.
  // Yet, these are not used anyway but in case they are planned to be used
  // then thy must be copied
  mesh->vertices = vertices;
  mesh->vertex_count = vertex_count;

  mesh->indices = indices;
  mesh->index_count = index_count;

  // Probably it makes more sense that it is allready packed to textures blob
  // Texture Array Pointer (texture_t**) should not be used,
  // Texture Object Array (texture*) should be used instead.
  // texture* can be looped using indexing syntax with the address-of operator
  if (textures != NULL) {
    mesh->textures = (texture_t*)malloc(4 * sizeof(texture_t));
    mesh->textures = textures[0];
    if (textures[1] != NULL) {
      texture_t* addr = &(mesh->textures[1]);
      memcpy(addr,  textures[1] , sizeof(texture_t));
    }
    if (textures[2] != NULL) {
      texture_t* addr = &(mesh->textures[2]);
      memcpy(addr,  textures[2], sizeof(texture_t));
    }
    if (textures[3] != NULL) {
      texture_t* addr = &(mesh->textures[3]);
      memcpy(addr,  textures[3], sizeof(texture_t));
    }
  }
  mesh->texture_count = texture_count;

  mesh->vao = vao_create();

  vao_bind(mesh->vao);

  vbo_t* vbo = vbo_create(mesh->vertices, mesh->vertex_count);
  ebo_t* ebo = ebo_create(mesh->indices, mesh->index_count);

	// Links VBO attributes such as coordinates and colors to VAO
	vao_link_attrib(mesh->vao, vbo, 0, 3, GL_FLOAT, sizeof(vertex_t), (void*)0);                   // position
	vao_link_attrib(mesh->vao, vbo, 1, 3, GL_FLOAT, sizeof(vertex_t), (void*)(3 * sizeof(float))); // color
	vao_link_attrib(mesh->vao, vbo, 2, 2, GL_FLOAT, sizeof(vertex_t), (void*)(6 * sizeof(float))); // tex_UV
	vao_link_attrib(mesh->vao, vbo, 3, 1, GL_FLOAT, sizeof(vertex_t), (void*)(8 * sizeof(float))); // tex_id
	vao_link_attrib(mesh->vao, vbo, 4, 3, GL_FLOAT, sizeof(vertex_t), (void*)(9 * sizeof(float))); // normal
	// Unbind all to prevent accidentally modifying them

  vao_unbind(mesh->vao);
  vbo_unbind(vbo);
  ebo_unbind(ebo);

  return mesh;
}

void mesh_draw(mesh_t* mesh, shader_t* shader, camera_t* camera) {

  shader_activate(shader);

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < mesh->texture_count; i++)
	{
    char* tex0;
    if (i == 0) {
      tex0 = "tex0[0]";
    }
    if (i == 1) {
      tex0 = "tex0[1]";
    }
		// std::string num;
		// std::string type = textures[i].type;
		// if (type == "diffuse")
		// {
		// 	num = std::to_string(numDiffuse++);
		// }
		// else if (type == "specular")
		// {
		// 	num = std::to_string(numSpecular++);
		// }

    texture_unit(&(mesh->textures[i]), shader, tex0, i);
    texture_bind(&(mesh->textures[i]));
	}

	// Take care of the camera Matrix
	glUniform3f(
    glGetUniformLocation(shader->ID, "camPos"),
    camera->Position[0],
    camera->Position[1],
    camera->Position[2]
  );
  camera_matrix(camera, shader, "camMatrix");
  vao_bind(mesh->vao);


// printf("incnt: %lu\n", mesh->index_count);
	// Draw the actual mesh

// printf("NEW to draw about %d triangles\n", mesh->index_count / sizeof(GLuint));
	glDrawElements(GL_TRIANGLES, mesh->index_count / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}