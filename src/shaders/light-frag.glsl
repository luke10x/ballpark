#version $VERSION

precision mediump float;
out vec4 fragColor;

uniform vec4 lightColor;

void main() {
    fragColor = lightColor;
}
