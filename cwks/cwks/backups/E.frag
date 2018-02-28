#version 400

in vec3 color;
in vec2 uv;

//uniform sampler2D uTex;

uniform int multiplicationFactor = 8;
uniform float threshold = 0.1;

void main() 
{
	// multiplicationFactor scales the number of stripes
    vec2 t = uv * multiplicationFactor ;
 
    // the threshold constant defines the with of the lines
    if (fract(t.s) < threshold  || fract(t.t) < threshold )
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);    
    else
        discard;



//    if (fract(uv.s * multiplicationFactor) < 0.5)
//        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); 
//    else
//        gl_FragColor = vec4(1.0, 1.0, 0.5, 1.0);



	//gl_FragColor = vec4(uv, 0.0, 1.0);
}