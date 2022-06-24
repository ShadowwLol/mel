#version 400 core

in vec4  o_color;  // output color
in vec2  o_coords; // output texture coordinates
in float o_slot;   // output texture slot

out vec4 FragColor;

uniform sampler2D u_Textures[10];

void main(){
        FragColor = texture(u_Textures[int(o_slot)], o_coords) * o_color;
}
