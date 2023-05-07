#version $VERSION

in vec4 aPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main() {
  gl_Position = camMatrix * model * vec4(aPos.rgb, 1);
}
