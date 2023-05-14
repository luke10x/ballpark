#version $VERSION

precision mediump float;
out vec4 fragColor;

in vec3 crntPos;
in vec3 color;
in vec2 texCoord;
in vec3 Normal;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;

// for specular
uniform vec3 camPos;

void main() {

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8.0);
	float specular = specAmount * specularLight;

	// outputs final color
	fragColor = texture(tex0, texCoord) * vec4(vec3(lightColor * (diffuse + ambient + specular)), 1.0f);
}
