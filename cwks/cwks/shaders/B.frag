#version 400 core



// ins
in vec3 o_color;
in vec3 o_norm;

// uniforms
uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_light_pos;

uniform vec3 u_ambient_color;
uniform vec3 u_diffuse_color;



void main() 
{	
// work out after interpolation
	vec3 L = normalize(u_light_pos);
	vec3 N = normalize(o_norm);

// calculate ambient effects
	vec3 amb = u_ambient_color;

// calculate diffuse effects
	vec3 dif = u_diffuse_color * clamp(dot(N, L), 0, 1);

// calculate final phong color
	vec3 final_color = o_color * (amb + dif);

// apply fragment color
	gl_FragColor = vec4(final_color,1);
}
