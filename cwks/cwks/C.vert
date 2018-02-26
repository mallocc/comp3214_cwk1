#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 light;

varying float d;
varying vec3 vNormal;

out vec3 color;

void main()
{
	gl_Position = P * V * M * vec4(vertexPosition_modelspace,1);  
	
	vNormal = vertexNormal;  

	color = vertexColor;
}