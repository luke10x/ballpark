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
	float ambient = 0.40f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.3f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 4.0);
	float specular = specAmount * specularLight;

	// outputs final color
	vec4 color;
	if (texId == 0.0) {
		color = texture(tex0[0], texCoord);
	} else if (texId == 1.0) {
		color = texture(tex0[1], texCoord);
	} else if (texId == 2.0) {
		color = texture(tex0[2], texCoord);
	} else {
		color = texture(tex0[3], texCoord);
	}

	fragColor = color * vec4(vec3(lightColor * (diffuse + ambient + specular)), 1.0f);
}
