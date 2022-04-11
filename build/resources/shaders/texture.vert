#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float a_TexIndex;

out vec4 ourColor;
out vec2 TexCoord;
out float v_TexIndex;

void main(){
	gl_Position = vec4(aPos, 0.0f, 1.0f);

	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	v_TexIndex = a_TexIndex;
}
