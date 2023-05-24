#version $VERSION

precision mediump float;
out vec4 fragColor;

in vec3 crntPos;
in vec3 color;
in vec2 texCoord;
in float texId;

in vec3 Normal;

uniform sampler2D tex0[4];
uniform vec4 lightColor;
uniform vec3 lightPos;

// for specular
uniform vec3 camPos;

void main() {

	// ambient lighting
	float ambient = 0.8f;

	vec4 color = texture(tex0[3], texCoord);

	fragColor = vec4(vec3(color * ambient), 1.0);
}
