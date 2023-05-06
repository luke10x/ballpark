#include <stdio.h>
#include <string.h>

#include "ppm.h"


void ppm_load(const char* file_basename, ppm_t** result, int flip) {
  const char* directory_path = "./assets/ppm/";
  const char* extension = ".ppm";
  char* filename = malloc(strlen(directory_path) + strlen(file_basename) + strlen(extension) + 1);
  sprintf(filename, "%s%s%s", directory_path, file_basename, extension);

  // Even if falis to load we want to return an empty material 
  (*result)->name = strdup(file_basename);
  (*result)->pixels = NULL;
  (*result)->max_val = 0;
  (*result)->width = 0;
  (*result)->height = 0;

  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Error opening file %s\n", filename);

    return;
  }

  // Read PPM header
  char magic[3];
  int width, height, max_val;
  
  if (fscanf(fp, "%2s", magic) != 1 || strcmp(magic, "P6") != 0) {
      fprintf(stderr, "Invalid magic number %s\n", filename);
      return;
  }
  // Skip over any comments or blank lines after magic number
  int c;
  while ((c = fgetc(fp)) == ' ' || c == '\n' || c == '#') {
      if (c == '#') {
          // Skip over comment line
          while ((c = fgetc(fp)) != '\n' && c != EOF) {}
      }
  }

  // Read dimensions and max_val from PPM file
  ungetc(c, fp);

  // read dimensions and maximum value
  int nread = fscanf(fp, "%d", &width);
  if (nread != 1) {
      fprintf(stderr, "Could not read width %s\n", filename);
      return;
  }
  while ((c = fgetc(fp)) == '\n'); // skip newlines
  ungetc(c, fp);

  nread = fscanf(fp, "%d", &height);
  if (nread != 1) {
      fprintf(stderr, "Could not read height %s\n", filename);
      return;
  }
  while ((c = getc(fp)) == '\n'); // skip newlines
  ungetc(c, fp);

  nread = fscanf(fp, "%d", &max_val);
  if (nread != 1) {
      fprintf(stderr, "Could not read maximum value %s\n", filename);
      return;
  }
  while ((c = getc(fp)) == '\n'); // skip newlines
  ungetc(c, fp);

  // Allocate memory for pixel data
  pixel_t* pixels = malloc(sizeof(pixel_t) * width * height);
  if (!pixels) {
    fprintf(stderr, "Error allocating memory for pixels\n");
    fclose(fp);
    return;
  }

  // Read pixel data
  int i = 0;
  uint8_t buffer[3];
  int row_offset = (flip ? height - 1 : 0);
  int row_step = (flip ? -1 : 1);
  for (int row = row_offset; row >= 0 && row < height; row += row_step) {
    for (int col = 0; col < width && fread(buffer, 1, 3, fp) == 3; col++) {
      pixels[row * width + col].r = buffer[0] * 255 / max_val;
      pixels[row * width + col].g = buffer[1] * 255 / max_val;
      pixels[row * width + col].b = buffer[2] * 255 / max_val;
      i++;
    }
  }

  (*result)->name = strdup(file_basename);
  (*result)->pixels = pixels;
  (*result)->max_val = max_val;
  (*result)->width = width;
  (*result)->height = height;

  fclose(fp);
}

void ppm_delete(ppm_t* ppm) {
  free(ppm->pixels);
  free(ppm->name);
  free(ppm);
}
