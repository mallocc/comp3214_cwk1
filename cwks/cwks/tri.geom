#version 400
precision highp float;
uniform mat4 mvpmatrix;
layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;
out vec3 ex_Color;
in vec3 geom_Color[3];
void main() {
    int i;
    for (i=0; i < gl_in.length(); i++)  {
        gl_Position = mvpmatrix * vec4((gl_in[i].gl_Position.xyz + vec3(0., 0., -3.)), 1.0);
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
    for (i=0; i < gl_in.length(); i++) {
        gl_Position = mvpmatrix * vec4((gl_in[i].gl_Position.xyz + vec3(0., 0., 0.0)), 1.0);
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
    for (i=0; i < gl_in.length(); i++) {
        gl_Position = mvpmatrix * vec4((gl_in[i].gl_Position.xyz + vec3(0., 0., +3.0)), 1.0);
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
}