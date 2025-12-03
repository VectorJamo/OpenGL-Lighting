#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tCoord;
out float fogIntensity;

void main()
{
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);

	// Fog calculation
	vec4 viewSpacePosition = view * model * vec4(position.xyz, 1.0);

	// Simple linear fog
	float dist = length(viewSpacePosition.xyz);
	float fogValue = 0;
	if(dist < 300.0)
	{
		fogValue = 0.0;
	}else if(dist > 300.0 && dist < 1000.0){
		fogValue = dist/1000.0;
	}else{
		fogValue = 1.0f;
	}

	fogIntensity = fogValue;

	tCoord = textCoord;
}