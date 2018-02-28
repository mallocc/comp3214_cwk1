#version 400
precision highp float;
in  vec3 vEx_Color;
out vec4 FragColor;
void main(void) {
    FragColor = vec4(vEx_Color,1.0);
}
