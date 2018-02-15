#version 330
//precision highp float;
	
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 color;

varying vec3 vNormal;
varying vec3 vViewPosition;

#pragma glslify: transpose = require('glsl-transpose')
#pragma glslify: inverse = require('glsl-inverse')

void main()
{
	mat4 model_view_matrix = V * M;
	vec4 view_model_pos = model_view_matrix * vec4(vertexPosition_modelspace,1);
	vViewPosition = view_model_pos.xyz;
	gl_Position = P * view_model_pos;  
	  
	mat3 normal_matrix = transpose(inverse(mat3(model_view_matrix)));
	vNormal = normalize(normal_matrix * vertexNormal);

	color = vertexColor;
}