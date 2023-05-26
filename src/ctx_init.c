#include <stdio.h>

#include "gl.h"

#include <GLFW/glfw3.h>

void ctx_init_video() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    return;
  }

  //if (!GLEW_VERSION_2_1)
//	  exit(1);
  	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4); // enable multisampling

}

GLFWwindow* ctx_init_window(int width, int height) {

      GLFWwindow* window = glfwCreateWindow(width, height, "Esports Ballpark", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
	  fprintf(stderr, "Glew not ok: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	return window;
}
