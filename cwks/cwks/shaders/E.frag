#version 400

in vec3 color;
in vec2 uv;

varying vec3 vViewPosition;
varying vec3 vNormal;

uniform sampler2D uTex;

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
	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - vViewPosition;

	float light_distance = length(light_vec);	
	float falloff = brightness / (light_distance*light_distance);

	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(vViewPosition);            //eye direction

	float specular = pow(clamp(dot(v, -l),0,1), shininess) * specular_scale * falloff;
	vec3 diffuse = diffuse_color * clamp(dot(vNormal, l),0,1) * falloff;	

	vec3 colour = texture(uTex,uv).xyz * (diffuse + ambient_color) + specular;

	gl_FragColor = vec4(colour,1);
}