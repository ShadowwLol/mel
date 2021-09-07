#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 0) in vec4 aColor;
layout (location = 0) in vec4 aTex;

out vec3 color;
out vec2 texCoord;

uniform float scale;

void main(){
    TexCoords = vertex.zw;
    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    color = aColor;
    texCoord = aTex;
}