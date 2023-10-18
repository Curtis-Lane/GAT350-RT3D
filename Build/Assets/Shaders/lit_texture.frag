#version 430

in layout(location = 0) vec2 texcoord;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec4 color;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texcolor = texture(tex, texcoord);
	ocolor = texcolor * color;
}
