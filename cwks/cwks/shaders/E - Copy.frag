#version 400

in vec3 color;
in vec2 uv;

varying vec3 vViewPosition;
varying vec3 vNormal;
varying vec3 vBinormal;

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
	vec3 extra_tangent = cross(vNormal, vBinormal);
	mat3 tangent_space = mat3(
		vNormal,
		vBinormal,
		extra_tangent
	);
	vec3 converted_normal = texture2D(uNorm,uv).rgb * 2.0f - 1.0f;
	normalize(converted_normal);
	vec3 bumpmap_normal = tangent_space * converted_normal;
	normalize(bumpmap_normal);
	vec3 new_normal = vNormal + bumpmap_normal;
	normalize(new_normal);

	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - vViewPosition;

	float light_distance = length(light_vec);	
	float falloff = brightness / (light_distance*light_distance);

	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(vViewPosition);            //eye direction

	float specular = pow(clamp(dot(v, -l),0,1), shininess) * specular_scale * falloff;
	vec3 diffuse = diffuse_color * clamp(dot(new_normal, l),0,1) * falloff;	

	vec3 colour = texture2D(uTex,uv).rgb * (diffuse + ambient_color + specular);

	gl_FragColor = vec4(colour,1);
}