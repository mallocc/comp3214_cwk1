#version 400 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexColor;

layout(location = 1) in vec3 vertexNormal;
uniform vec3 light;

uniform mat4 MVP;
out vec3 fragmentColor;

void main()
{
	  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	  vec3 d =  light - gl_Position.xyz;

	  fragmentColor = vec3(1.0f,1.0f,1.0f) * clamp(dot(d,vertexNormal), 0, 1);
}