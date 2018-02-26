#version 400 core

in vec3 color;

varying vec3 vViewPosition;
varying vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;



void main() 
{

	gl_FragColor = vec4(color,1);
}
