#version 400

in vec3 color;
in vec2 uv;

uniform sampler2D uTex;

void main() 
{
	gl_FragColor = texture(uTex,uv);
}