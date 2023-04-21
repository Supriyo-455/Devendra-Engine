#version 430

layout(binding = 0) uniform sampler2D samp;

in vec4 varyingColor;
in vec2 tc; // texture coordinates

uniform mat4 mv_Matrix;
uniform mat4 proj_Matrix;

out vec4 color;

void main()
{
	color = varyingColor * 0.5 + texture(samp, tc);
}