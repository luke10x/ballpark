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
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef __EMSCRIPTEN__
  // OpenGL ES 2.0
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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