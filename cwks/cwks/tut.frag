#version 400 core
precision highp float;

//out vec3 color;
//uniform vec3 fragmentColor;

in vec3 color;

varying vec3 vViewPosition;
varying vec3 vNormal;

uniform vec3 light;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

const float specularScale = 0.5;
const float shininess = 1.0;
const float roughness = 1.0;
const float albedo = 0.95;


void main() {



	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - vViewPosition;
	float light_distance = length(light_vec);
	float falloff = 1;

	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(vViewPosition);            //eye direction
	
	vec3 reflection = 2.0 * dot(vNormal,light_vec) * vNormal - light_vec;

	float cos_angle = dot(reflection, -light_vec);
	cos_angle = clamp(cos_angle, 0.0, 1.0);
	cos_angle = pow(cos_angle, shininess);

	float specular = pow(clamp(dot(vNormal, light_vec),0,1), shininess) * specularScale * falloff;
	vec3 diffuse = vec3(0,1,1) * clamp(dot(vNormal, light_vec),0,1) * falloff;
	vec3 ambient = vec3(.1,.1,.1);

	vec3 colour = color * (diffuse + ambient) + specular;

	gl_FragColor = vec4(colour,1);
}
