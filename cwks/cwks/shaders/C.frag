#version 400

in vec3 color;

varying vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 light;
uniform vec3 ambient_color;
uniform vec3 diffuse_color;

void main() 
{
	vec3 diffuse = diffuse_color * clamp(dot(vNormal, normalize(light)),0,1);	

	vec3 colour = color * (diffuse + ambient_color);

	gl_FragColor = vec4(colour,1);
}
