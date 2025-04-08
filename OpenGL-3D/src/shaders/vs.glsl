#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertNormal;
out vec3 fragWorldPosition;

void main()
{
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);

	vertNormal = normal;
	vec4 worldPositionVector = model * vec4(position.xyz, 1.0);
	
	fragWorldPosition = vec3(worldPositionVector.xyz);
}