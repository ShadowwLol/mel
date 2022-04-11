#version 330 core

in vec4 ourColor;
in vec2 TexCoord;
in float v_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[10];

void main(){
	int index = int(v_TexIndex);
	switch(index){
		case 0:
			FragColor = texture(u_Textures[0], TexCoord) * ourColor;
			break;
		case 1:
			FragColor = texture(u_Textures[1], TexCoord) * ourColor;
			break;
		case 2:
			FragColor = texture(u_Textures[2], TexCoord) * ourColor;
			break;
		case 3:
			FragColor = texture(u_Textures[3], TexCoord) * ourColor;
			break;
		case 4:
			FragColor = texture(u_Textures[4], TexCoord) * ourColor;
			break;
		case 5:
			FragColor = texture(u_Textures[5], TexCoord) * ourColor;
			break;
		case 6:
			FragColor = texture(u_Textures[6], TexCoord) * ourColor;
			break;
		case 7:
			FragColor = texture(u_Textures[7], TexCoord) * ourColor;
			break;
		case 8:
			FragColor = texture(u_Textures[8], TexCoord) * ourColor;
			break;
		case 9:
			FragColor = texture(u_Textures[9], TexCoord) * ourColor;
			break;
	}
}
