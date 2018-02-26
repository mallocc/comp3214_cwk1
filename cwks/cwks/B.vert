#version 400 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 light;
uniform float specular_scale;
uniform float shininess;
uniform float brightness;

uniform vec3 ambient_color;
uniform vec3 diffuse_color;

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

	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - vViewPosition;
	float light_distance = length(light_vec);	
	float falloff = brightness / (light_distance*light_distance);

	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(vViewPosition);            //eye direction

	float specular = pow(clamp(dot(v, -l),0,1), shininess) * specular_scale * falloff;
	vec3 diffuse = diffuse_color * clamp(dot(vNormal, l),0,1) * falloff;	

	vec3 colour = color * (diffuse + ambient_color);// + specular;



	color = colour;
}