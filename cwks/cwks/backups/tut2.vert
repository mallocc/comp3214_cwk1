#version 400
in  vec2 aIn_Position;
in  vec3 aIn_Color;
out vec3 vEx_Color;
void main(void) {
    gl_Position = vec4(aIn_Position.x, aIn_Position.y, 0.0, 1.0);
    vEx_Color = aIn_Color;
}
