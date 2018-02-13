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
uniform vec3 eye_pos;

const float specularScale = 0.2;
const float shininess = 20.0;
const float roughness = 1.0;
const float albedo = 0.95;



void main() {

	

	vec4 light_pos = V * vec4(light,1);
	vec3 light_vec = light_pos.xyz - vViewPosition;
	float light_distance = length(light_vec);
	float brightness = 30;
	float falloff = brightness / (light_distance*light_distance);

	vec3 l = normalize(light_vec);              //light direction
	vec3 v = normalize(vViewPosition);            //eye direction

	float specular = pow(clamp(dot(v, l),0,1), shininess) * specularScale * falloff;
	vec3 diffuse = vec3(1,1,1) * clamp(dot(vNormal, -l),0,1) * falloff;
	vec3 ambient = vec3(.1,.1,.1);

	vec3 colour = color * (diffuse + ambient) + specular;

	gl_FragColor = vec4(colour,1);
}
