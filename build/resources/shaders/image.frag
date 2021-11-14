#version 330 core
#extension GL_ARB_explicit_uniform_location : require
layout (location = 3) uniform sampler2D sampler;

in vec4 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	FragColor = texture(sampler, TexCoord) * ourColor;
}
