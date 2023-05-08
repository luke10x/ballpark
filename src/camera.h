#pragma once

#include "shader.h"

#include <cglm/cglm.h>
#include <math.h>

typedef struct {
  vec3 Position;
  vec3 Orientation;
  vec3 Up;

  mat4 camera_matrix;

	// Prevents the camera from jumping around when first clicking left click
	int firstClick;

  int width;
  int height;

  float speed;
  float sensitivity;

} camera_t;

camera_t* camera_create(int width, int height, vec3 position);

void camera_update_matrix(
  camera_t* self,
  float fov_deg,
  float near_plane,
  float far_plane
);

void camera_matrix(
  camera_t* self,
  shader_t* shader,
  const char* uniform
);

void camera_inputs(camera_t* self, GLFWwindow* window);
