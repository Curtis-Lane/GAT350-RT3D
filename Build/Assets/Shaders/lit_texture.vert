#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texcoord;
in layout(location = 2) vec3 normal;

out layout(location = 0) vec2 otexcoord;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material {
	vec4 color;
	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	vec3 position;
	vec3 color;
} light;

uniform vec3 ambientLight;

void main()
{
	otexcoord = (texcoord * material.tiling) + material.offset;
	onormal = normal;

	mat4 modelView = view * model;
	vec4 lightMVPosition = modelView * vec4(position, 1);
	vec3 lightNormal = normalize(mat3(modelView) * normal);
	vec3 lightDir = normalize(light.position - lightMVPosition.xyz);
	float intensity = max(dot(lightDir, lightNormal), 0);

	vec3 lightColor = (light.color * intensity) + ambientLight;
	ocolor = material.color * vec4(lightColor, 1);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(position, 1.0);
}
