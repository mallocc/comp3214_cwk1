#version 400

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec2 inUV;
layout(location = 4) in vec3 inTang;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 color;
out vec2 uv;
out vec3 view_pos;
out vec3 norm;
out vec3 tang;

void main()
{
	vec4 vp = V*M*vec4(inVert,1);	
	gl_Position = P*vp;
	  
	view_pos = vp.xyz;

	vec4 new_norm = M * vec4(inNorm,1);
	norm = new_norm.xyz;

	vec4 new_tang = M * vec4(inTang,1);
	tang = new_tang.xyz;

	color = inColor;

	uv = inUV;
}