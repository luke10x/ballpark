#version 300 es

precision mediump float;

in vec3 aPos;
in vec3 aColor;
in vec2 aTex;
in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
   crntPos = vec3(model * vec4(aPos, 1.0f));
   gl_Position = camMatrix * vec4(crntPos, 1.0f);

   color = aColor;
   texCoord = aTex;
   Normal = aNormal;
}