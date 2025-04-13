#version 330 core

in vec2 tCoord;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D textureUnit;

uniform vec3 directionalLightDir;
uniform vec3 pointLight1Pos;
uniform vec3 pointLight2Pos;
uniform vec3 eyePosition;

uniform vec3 pointLightColor;
uniform vec3 directionalLightColor;

vec3 CalculateDirectionalLight();

void main()
{
	vec3 directionalLight = CalculateDirectionalLight();

	color = vec4(directionalLight, 1.0) * texture(textureUnit, tCoord);
}

vec3 CalculateDirectionalLight()
{
	vec3 fragmentColor;
	
	// Ambient light
	float ambientStrength = 0.2;
	fragmentColor = directionalLightColor * ambientStrength;

	// Diffuse light
	float diffuseStrength = max(dot(normalize(-directionalLightDir), fragNormal), 0.0);
	vec3 diffuseLight = diffuseStrength * directionalLightColor;

	fragmentColor += diffuseLight;

	return fragmentColor;
}