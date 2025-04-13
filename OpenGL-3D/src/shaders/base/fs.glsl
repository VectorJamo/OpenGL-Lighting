#version 330 core

in vec2 tCoord;
out vec4 color;

uniform sampler2D textureUnit;

void main()
{
	color = texture(textureUnit, tCoord);
}
