#version 330 core
out vec4 FragColor;

in vec3 o_Color;

void main(){
    FragColor = vec4(o_Color, 1.0);
}
