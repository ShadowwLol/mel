#version 400 core
layout (location = 0) in vec2  i_pos;    // input position
layout (location = 1) in vec4  i_color;  // input color
layout (location = 2) in vec2  i_coords; // input texture coordinates
layout (location = 3) in float i_slot;   // input texture slot

out vec4  o_color;  // output color
out vec2  o_coords; // output texture coordinates
out float o_slot;   // output texture slot

void main(){
	gl_Position = vec4(i_pos, 0.0f, 1.0f);

        o_color  = i_color;
        o_coords = i_coords;
        o_slot   = i_slot;
}
