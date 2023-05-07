#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GLFW/glfw3.h>

void ctx_init_video() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    return;
  }

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef __EMSCRIPTEN__
  glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); // enable multisampling
#endif
}

GLFWwindow* ctx_init_window(int width, int height) {

  GLFWwindow* window = glfwCreateWindow(width, height, "YoutOpenGL", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}
  glfwMakeContextCurrent(window);

	return window;
}