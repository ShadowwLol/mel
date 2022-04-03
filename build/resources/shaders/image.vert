#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in mat4 mvp;

out vec4 ourColor;
out vec2 TexCoord;
out float v_TexIndex;

void main(){
	gl_Position = mvp * vec4(aPos, 1.0);

	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	v_TexIndex = a_TexIndex;
}
