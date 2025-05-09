// Fragment Shader for the light object

#version 330 core
out vec4 color;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor.xyz, 1.0);
}
