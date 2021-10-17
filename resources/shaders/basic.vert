#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 projection;

out vec4 o_Color;

void main(){
    gl_Position = projection * vec4(aPos.xyz, 1.0);
	o_Color = aColor;
}
