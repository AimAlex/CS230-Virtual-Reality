#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 5) in vec3 color;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;

out vec2 TexCoord;
out vec4 vertColor;

void main() {
	TexCoord = texCoords;
	gl_Position = Proj * View * Model *  vec4(position, 1.0);
	vertColor = vec4(color,1.0);
}
