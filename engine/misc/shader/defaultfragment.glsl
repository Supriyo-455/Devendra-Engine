#version 330 core
#define PI 3.14159
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
in vec2 TexCoord;


uniform float time;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void) {
    //float wave = sin(time);
    // Put it all together
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.5) * vertexColor * time;
}