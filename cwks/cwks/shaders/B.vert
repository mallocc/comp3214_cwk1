#version 400 core



// ins
layout(location = 0) in vec3 i_vert;
layout(location = 1) in vec3 i_color;
layout(location = 2) in vec3 i_norm;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_light_pos;
uniform vec3 u_eye_pos;

// outs
out vec3 o_color;
out vec3 o_norm;



void main()
{
// calculate world position of vertex
	vec4 world_pos = u_m * vec4(i_vert, 1.0f);

// normal in world space
	vec4 new_norm  = u_m * vec4(i_norm,1);
	o_norm         = new_norm.xyz;

// color of vertex
	o_color        = i_color;

// set projected point
	gl_Position    = u_p * u_v * world_pos;	
}