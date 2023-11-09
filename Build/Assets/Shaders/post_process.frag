#version 430

#define INVERT_MASK    1
#define GRAYSCALE_MASK 2
#define COLORTINT_MASK 4

in layout(location = 0) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D screenTex;

uniform float blend = 1;
uniform uint params = 0;

vec4 invert(in vec4 color) {
	return vec4(vec3(1) - color.rbg, color.a);
}

vec4 grayscale(in vec4 color) {
	return vec4(vec3((color.r + color.g + color.b) * 0.33333333333333333333333333333333), color.a);
}

void main()
{
	vec4 basecolor = texture(screenTex, texcoord);
	vec4 post_process = basecolor;

	if(bool(params & INVERT_MASK)) {
		post_process = invert(post_process);
	}
	if(bool(params & GRAYSCALE_MASK)) {
		post_process = grayscale(post_process);
	}
	if(bool(params & COLORTINT_MASK)) {
		//
	}

	ocolor = mix(basecolor, post_process, blend);
}
