#version 330 core
#define PI 3.14159
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
uniform vec4 ourColor; // we set this variable in the OpenGL code.
uniform float time;

void main(void) {
    // Have something to vary the radius (can also just be a linear counter (time))
    float wave = sin(time);

    // vec4 color1 = vec4(1.0, 0.0, 0.0, 1.0); // Red
    // vec4 color2 = vec4(0.0, 0.0, 1.0, 1.0); // Blue
    // vec4 color3 = vec4(0.0, 1.0, 0.0, 1.0); // Green
    // vec4 color4 = mix(color2, color3, sin(2*PI*time));
    // vec4 color5 = mix(color1, color2, sin(2*PI*time));

    // Put it all together
    FragColor = vertexColor*wave;
}