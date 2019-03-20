#version 330 core

layout(location = 0) out vec4 outColor;

uniform sampler2D texture1;
uniform int useDiffuseTex;

in vec2 TexCoord;
in vec4 vertColor;

void main() {
	outColor = vertColor;
	if(bool(useDiffuseTex))
	{ 
		outColor = vec4(texture(texture1, TexCoord));
	}
}
