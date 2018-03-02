#version 400

in vec3 color;
in vec2 uv;
in vec3 view_pos;
in vec3 norm;
in vec3 tang;

uniform sampler2D uTex;
uniform sampler2D uNorm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 light;
uniform float specular_scale;
uniform float shininess;
uniform float brightness;

uniform vec3 ambient_color;
uniform vec3 diffuse_color;

void main() 
{
// find the tangent vector space
	vec3 binorm = cross(norm, tang);
	mat3 tangent_space = mat3(
		norm,
		tang,
		binorm
	);

	vec3 converted_normal_map = texture2D(uNorm,uv).rgb * 2.0f - 1.0f;
	converted_normal_map = normalize(converted_normal_map);
	vec3 worldspace_normal_map = tangent_space * converted_normal_map;
	worldspace_normal_map = normalize(worldspace_normal_map);

	vec3 new_normal = norm + worldspace_normal_map;
	new_normal = normalize(new_normal);

	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - view_pos;

	float light_distance = length(light_vec);	
	float falloff = brightness / (light_distance*light_distance);


	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(view_pos);            //eye direction

	vec3 reflection = reflect(l, norm);

	float specular = pow(clamp(dot(v, reflection),0,1), shininess) * specular_scale * falloff;	

	vec3 diffuse = diffuse_color * clamp(dot(new_normal, l),0,1) * falloff;	

	vec3 final_color = texture2D(uTex,uv).rgb * ( diffuse + ambient_color ) + specular;

	gl_FragColor = vec4(final_color,1);
}