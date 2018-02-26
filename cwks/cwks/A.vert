#version 400 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNorm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 color;

void main()
{
	gl_Position = P * V * M * vec4(inVert, 1);
	color = inColor;
}