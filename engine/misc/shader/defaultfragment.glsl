#version 330 core
#define PI 3.14159
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
uniform float time;

void main(void) {
    float wave = sin(time);
    // Put it all together
    FragColor = vertexColor*wave;
}