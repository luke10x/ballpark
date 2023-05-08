/*
** Context is the initialization data, and data that
** has to persist between frames
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "buffers.h"
#include "ppm.h"
#include "camera.h"
#include "texture.h"

#include "ctx_init.c"

typedef struct {
  int should_continue;
  GLFWwindow* window;
  shader_t* default_shader;
  GLsizei default_count;
  vao_t* pyramid_vao;
  
  GLsizei light_count;
  shader_t* light_shader;
  vao_t* light_vao;

  camera_t* camera;
  float aspect;
} ctx_t;

/*
 * Constructor, it is called before main loop starts
 * It initializes the video, and creates the window too.
 */
inline static void ctx_resize_framebuffer_to_window(ctx_t* c);

ctx_t* ctx_create() {
  ctx_init_video();

  ctx_t* ctx = malloc(sizeof(ctx_t));
  ctx->should_continue = 1;
  ctx->window = ctx_init_window(320, 240);

  ctx_resize_framebuffer_to_window(ctx);

  // 💜 Shaders
  ctx->default_shader = shader_create("src/shaders/default-vert.glsl", "src/shaders/default-frag.glsl");
  ctx->light_shader = shader_create("src/shaders/light-vert.glsl", "src/shaders/light-frag.glsl");

  // Vertices coordinates
  vertex_t vertices[] =
  {
    { .position = {-0.5f, 0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 0.0f, 0.0f }, .texUV={ -1.0f, 0.0f}}, // Bottom side
    { .position = {-0.5f, 0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 5.0f, 0.0f }, .texUV={ -1.0f, 0.0f}}, // Bottom side
    { .position = {0.5f,  0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 5.0f, 0.0f }, .texUV={ -1.0f, 0.0f}}, // Bottom side
    { .position = {0.5f,  0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 0.0f, 0.0f }, .texUV={ -1.0f, 0.0f}}, // Bottom side
    { .position = {-0.5f, 0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 0.0f,-0.8f }, .texUV={ 0.5f,  0.0f}}, // Left Side
    { .position = {-0.5f, 0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 0.0f,-0.8f }, .texUV={ 0.5f,  0.0f}}, // Left Side
    { .position = {0.0f,  0.8f,  0.0f}, .normal = {0.92f, 0.86f, 0.76f}, .color = { 2.5f, 5.0f,-0.8f }, .texUV={ 0.5f,  0.0f}}, // Left Side
    { .position = {-0.5f, 0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 0.0f, 0.0f }, .texUV={ 0.5f, -0.8f}}, // Non-facing side
    { .position = {0.5f,  0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 0.0f, 0.0f }, .texUV={ 0.5f, -0.8f}}, // Non-facing side
    { .position = {0.0f,  0.8f,  0.0f}, .normal = {0.92f, 0.86f, 0.76f}, .color = { 2.5f, 5.0f, 0.0f }, .texUV={ 0.5f, -0.8f}}, // Non-facing side
    { .position = {0.5f,  0.0f, -0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 0.0f, 0.8f }, .texUV={ 0.5f,  0.0f}}, // Right side
    { .position = {0.5f,  0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 0.0f, 0.8f }, .texUV={ 0.5f,  0.0f}}, // Right side
    { .position = {0.0f,  0.8f,  0.0f}, .normal = {0.92f, 0.86f, 0.76f}, .color = { 2.5f, 5.0f, 0.8f }, .texUV={ 0.5f,  0.0f}}, // Right side
    { .position = {0.5f,  0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 5.0f, 0.0f, 0.0f }, .texUV={ 0.5f,  0.8f}}, // Facing side
    { .position = {-0.5f, 0.0f,  0.5f}, .normal = {0.83f, 0.70f, 0.44f}, .color = { 0.0f, 0.0f, 0.0f }, .texUV={ 0.5f,  0.8f}}, // Facing side
    { .position = {0.0f,  0.8f,  0.0f}, .normal = {0.92f, 0.86f, 0.76f}, .color = { 2.5f, 5.0f, 0.0f }, .texUV={ 0.5f,  0.8f}}  // Facing side
  };
  // Indices for vertices order
  GLuint indices[] =
  {
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
  };

  vertex_t lightVertices[] =
  { //     COORDINATES     //
    { .position = {-0.1f, -0.1f,  0.1f}},
    { .position = {-0.1f, -0.1f, -0.1f}},
    { .position = {0.1f, -0.1f,  -0.1f}},
    { .position = {0.1f, -0.1f,   0.1f}},
    { .position = {-0.1f,  0.1f,  0.1f}},
    { .position = {-0.1f,  0.1f, -0.1f}},
    { .position = {0.1f,  0.1f,  -0.1f}},
    { .position = {0.1f,  0.1f,   0.1f}}
  };

  GLuint lightIndices[] =
  {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
  };

	// Generates Vertex Array Object and binds it
	vao_t* VAO1 = vao_create();

  vao_bind(VAO1);

  // Generates Vertex Buffer Object and links it to vertices
	vbo_t* VBO1 = vbo_create(vertices, sizeof(vertices));

  // Generates Element Buffer Object and links it to indices
	ebo_t* EBO1 = ebo_create(indices, sizeof(indices));

  // Links VBO to VAO
  vao_link_attrib(VAO1, VBO1, 0, 3, GL_FLOAT, sizeof(vertex_t), (void*) 0);                  // Coordinates
  vao_link_attrib(VAO1, VBO1, 1, 3, GL_FLOAT, sizeof(vertex_t), (void*)(3 * sizeof(float))); // Colors
  vao_link_attrib(VAO1, VBO1, 2, 2, GL_FLOAT, sizeof(vertex_t), (void*)(6 * sizeof(float))); // TexCoord
  vao_link_attrib(VAO1, VBO1, 3, 3, GL_FLOAT, sizeof(vertex_t), (void*)(8 * sizeof(float))); // Normals


  vao_unbind(VAO1);
  vbo_unbind(VBO1);
  ebo_unbind(EBO1); 

  // Now same for light
	// Generates Vertex Array Object and binds it
	vao_t* lightVAO = vao_create();
	vao_bind(lightVAO);


	// Generates Vertex Buffer Object and links it to vertices
	vbo_t* lightVBO = vbo_create(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	ebo_t* lightEBO = ebo_create(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	vao_link_attrib(lightVAO, lightVBO, 0, 3, GL_FLOAT, sizeof(vertex_t), (void*)0);
	// Unbind all to prevent accidentally modifying them
	vao_unbind(lightVAO);
	vbo_unbind(lightVBO);
	ebo_unbind(lightEBO);

	vec4 lightColor;
  glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.7f }, lightColor);

  vec3 lightPos;
	glm_vec3_copy((vec3){ 0.5f, 0.5f, 0.5f }, lightPos);
	mat4 lightModel = GLM_MAT4_IDENTITY_INIT;
  glm_translate(lightModel, lightPos);

	vec3 pyramidPos;
  glm_vec3_copy((vec3){ -1.0f, 0.0f, -3.0f }, pyramidPos);
	mat4 pyramidModel = GLM_MAT4_IDENTITY_INIT;
	glm_translate(pyramidModel, pyramidPos);

  // Texture
  texture_t* pop_cat = texture_create(
    // "04-12-wall",
    // "02-04-wall",
    "05-01-wall",
     GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE
  );

  
  texture_bind(pop_cat);

	shader_activate(ctx->light_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->light_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)lightModel);
	glUniform4f(glGetUniformLocation(ctx->light_shader->ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
  texture_unit(pop_cat, ctx->default_shader, "tex0", 0);
	
  shader_activate(ctx->default_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)pyramidModel);
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
	glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

  // Bind the VAO so OpenGL knows to use it
  vao_bind(VAO1);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

  int w, h;
  glfwGetFramebufferSize(ctx->window, &w, &h);

  camera_t* camera = camera_create(w, h, (vec3){0.0f, 0.0f, 2.0f});

	// Variables that help the rotation of the pyramid
	// ctx.rotation = 0.0f;
  ctx->default_count = sizeof(indices) / sizeof(int);
  ctx->light_count = sizeof(lightIndices) / sizeof(int);
  ctx->aspect = (float)w / h;
  ctx->camera = camera;
  ctx->pyramid_vao = VAO1;
  ctx->light_vao = lightVAO;


  // Shaders end 

  return ctx;
}

inline static void ctx_resize_framebuffer_to_window(ctx_t* ctx) {
  int w, h;
  glfwGetFramebufferSize(ctx->window, &w, &h);
  printf("Frame buffer size %dx%d\n", w, h);

  glViewport(0, 0, w, h);
}

/*
 * Performs one iteration of the main loop.
 *
 * - Checks for user input;
 * - updates application state accordingly,
 * - and triggers rendering if necessary.
 */
static void ctx_handle_input(ctx_t* ctx);
static void ctx_advance_state(ctx_t* ctx);
static void ctx_render(ctx_t* ctx);

void ctx_perform_one_cycle(ctx_t* ctx) {
  static int frames_rendered = 0;
  static double last_fps_print_time = 0.0;
  double current_time = glfwGetTime();
  
  // Calculate the elapsed time since the last FPS calculation
  double time_since_last_fps_print = current_time - last_fps_print_time;
  
  if (time_since_last_fps_print >= 5.0) {
    // Calculate the FPS and print it to the console
    double fps = frames_rendered / time_since_last_fps_print;
    printf("FPS: %f\n", fps);
    
    // Reset the frame counter and update the last FPS print time
    frames_rendered = 0;
    last_fps_print_time = current_time;

    // This makes OpenGL to adjust to window side changes
    // (More important for web as desktop somehow already handles it)
    ctx_resize_framebuffer_to_window(ctx);
  }
  
  // Handle input, advance state, and render the scene
  ctx_handle_input(ctx);
  ctx_advance_state(ctx);
  ctx_render(ctx);
  
  // Poll events from the window system's event queue
  glfwPollEvents();
  
  // Increment the frame counter
  frames_rendered++;
}

inline static void ctx_handle_input(ctx_t* ctx) {
  if (glfwGetKey(ctx->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(ctx->window, GLFW_TRUE);
  }
  if (glfwWindowShouldClose(ctx->window)) {
    ctx->should_continue = 0;
  }

  camera_inputs(ctx->camera, ctx->window);

}

inline static void ctx_advance_state(ctx_t* ctx) {
  camera_update_matrix(ctx->camera, 45.0f, 0.1f, 100.0f);
}

inline static void ctx_render(ctx_t* ctx) {
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Shaders

    // Tell OpenGL which Shader Program we want to use
    shader_activate(ctx->default_shader);


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(
      glGetUniformLocation(ctx->default_shader->ID, "camPos"),
      ctx->camera->Position[0],
      ctx->camera->Position[1],
      ctx->camera->Position[2]
      );

    camera_matrix(ctx->camera, ctx->default_shader, "camMatrix");
    // Draw primitives, number of indices, datatype of indices, index of indices
    vao_bind(ctx->pyramid_vao);
    glDrawElements(GL_TRIANGLES, ctx->default_count, GL_UNSIGNED_INT, 0);

		// Tells OpenGL which Shader Program we want to use
    shader_activate(ctx->light_shader);
		// Export the camMatrix to the Vertex Shader of the light cube
		camera_matrix(ctx->camera, ctx->light_shader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
    vao_bind(ctx->light_vao);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, ctx->light_count, GL_UNSIGNED_INT, 0);

    // End shaders

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(ctx->window);
}

/*
 * Destructor exits graphics and frees memory
 */
void ctx_delete(ctx_t* ctx) {
  printf("Destroying context...\n");
	glfwDestroyWindow(ctx->window);
	glfwTerminate();

  free(ctx);
}