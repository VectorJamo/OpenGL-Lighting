#version 330 core

in vec2 tCoord;
in float fogIntensity;
out vec4 color;

uniform sampler2D textureUnit;
uniform vec4 skyColor;

void main()
{
	vec4 outputColor = mix(texture(textureUnit, tCoord), skyColor, fogIntensity);

	color = outputColor;
}