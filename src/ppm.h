#pragma once

#include <stdint.h>

typedef struct {
  uint8_t r,g,b;
} pixel_t;

typedef struct {
  char* name;
  pixel_t* pixels;
  int max_val;
  int width, height;
} ppm_t;

void ppm_load(const char* file_basename, ppm_t** result, int flip);
