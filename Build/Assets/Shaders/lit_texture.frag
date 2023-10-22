#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	vec3 position;
	vec3 color;
} light;

uniform vec3 ambientLight;

vec3 ads(in vec3 position, in vec3 normal) {
	// Cache the ambient light
	vec3 ambient = ambientLight;

	// Diffuse light calculations
	vec3 lightDir = normalize(light.position - position);
	float intensity = max(dot(normal, lightDir), 0);
	vec3 diffuse = material.diffuse * (light.color * intensity);

	// Specular light calculations
	vec3 specular = vec3(0);
	// Skip calculations if no light is present
	if(intensity > 0) {
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	// Merge the lights together
	return ambient + diffuse + specular;
}

void main()
{
	// Get color from texture
	vec4 texcolor = texture(tex, texcoord);
	// Multiply texture color by lighting
	ocolor = texcolor * vec4(ads(position, normal), 1);
}