#include "mesh.h"

mesh_t* mesh_create(
  char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t* textures,
  GLsizei texture_count
) {

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
          printf("with indices: [ ");
          for (int i = 0; i < index_count/ sizeof(GLuint); i++) {
            GLuint idx = (indices[i]);
            printf("%d, ", idx);
          }
          printf("] //end\n");

  mesh_t* mesh = malloc(sizeof(mesh_t));

  mesh->vertices = vertices;
  mesh->vertex_count = vertex_count;

  mesh->indices = indices;
  mesh->index_count = index_count;

  mesh->textures = textures;
  mesh->texture_count = texture_count;

  mesh->vao = vao_create();

  vao_bind(mesh->vao);

  vbo_t* vbo = vbo_create(mesh->vertices, mesh->vertex_count);
  ebo_t* ebo = ebo_create(mesh->indices, mesh->index_count);

	// Links VBO attributes such as coordinates and colors to VAO
	vao_link_attrib(mesh->vao, vbo, 0, 3, GL_FLOAT, sizeof(vertex_t), (void*)0);
	vao_link_attrib(mesh->vao, vbo, 1, 3, GL_FLOAT, sizeof(vertex_t), (void*)(3 * sizeof(float)));
	vao_link_attrib(mesh->vao, vbo, 2, 2, GL_FLOAT, sizeof(vertex_t), (void*)(6 * sizeof(float)));
	vao_link_attrib(mesh->vao, vbo, 3, 3, GL_FLOAT, sizeof(vertex_t), (void*)(8 * sizeof(float)));
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
    texture_unit(&(mesh->textures[i]), shader, "tex0", i);
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