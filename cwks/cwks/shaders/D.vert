#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUv;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 color;
out vec2 uv;

void main()
{
	gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1);
	color = vertexColor;
	uv = vertexUv;
}