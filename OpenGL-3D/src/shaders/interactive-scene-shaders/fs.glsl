#version 330 core

in vec2 tCoord;
out vec4 color;

uniform sampler2D textureUnit;

uniform int isFloor;
uniform vec3 floorColor;

void main()
{
	if(isFloor == 0)
	{
		color = texture(textureUnit, tCoord);
	}else{
		color = vec4(floorColor.xyz, 1.0);
	}
}
