#version 300 es

precision mediump float;
out vec4 fragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 lightColor;uniform vec3 lightPos;

// for specular
uniform vec3 camPos;

void main() {

	// ambient lighting
	float ambient = 0.2;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specularLight = 0.5;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 8.0);
	float specular = specAmount * specularLight;

	// outputs final color
	fragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}
