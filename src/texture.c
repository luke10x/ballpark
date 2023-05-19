#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"
#include "texture.h"

texture_t* texture_create(
  const char* image,
  GLenum tex_type,
  GLenum slot,
  GLenum format,
  GLenum pixel_type
) {
  texture_t* self = malloc(sizeof(texture_t));
  self->type = tex_type;

  ppm_t* material = malloc(sizeof(ppm_t));
  ppm_load(image, &material, 1);

  int widthImg = material->width;
  int heightImg = material->height;
  int numColCh = 10;

  unsigned char* bytes = (unsigned char*) material->pixels;
//  printf("pixels %s %dx%d .. \n", bytes,widthImg, heightImg );
  glGenTextures(1, &(self->ID));
  glActiveTexture(slot);
  glBindTexture(self->type, self->ID);

  glTexParameteri(self->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(self->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(self->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(self->type, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(self->type, 0, GL_RGB, widthImg, heightImg, 0,
    GL_RGB, GL_UNSIGNED_BYTE, bytes);
  glGenerateMipmap(self->type);
    
  free(material->pixels);
  free(material);

  // The following line should be uncommented
  glBindTexture(self->type, 0);

  // Should work without this
  glBindTexture(self->type, self->ID);

  return self;
}

void texture_unit(texture_t* self, shader_t* shader, const char* uniform, GLuint unit) {
  GLuint tex_uni = glGetUniformLocation(shader->ID, uniform);
  shader_activate(shader);
  glUniform1i(tex_uni, unit);
}

void texture_bind(texture_t* self) {
  glBindTexture(self->type, self->ID);
}

void texture_unbind(texture_t* self) {
  glBindTexture(self->type, 0);
}

void texture_delete(texture_t* self) {
  glDeleteTextures(1, &(self->ID));
}
