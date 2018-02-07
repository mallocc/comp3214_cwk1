#version 400 core

out vec3 color;

in vec3 fragmentColor;

void main() {
	color = fragmentColor;
}
