#version 330 core

in vec2 tCoord;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 color;

// Uniforms
uniform vec3 directionalLightDir;
uniform vec3 pointLight1Pos;
uniform vec3 pointLight2Pos;
uniform vec3 eyePos;

uniform vec3 pointLightColor;
uniform vec3 directionalLightColor;

// Materials
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	int shininess;
};

uniform Material material;

// Functions
vec3 CalculateDirectionalLight();

void main()
{
	vec3 directionalLight = CalculateDirectionalLight();

	color = vec4(directionalLight, 1.0);
}

vec3 CalculateDirectionalLight()
{
	vec3 fragmentColor;
	
	// Ambient light
	vec3 ambientLightStrength = 0.2 * directionalLightColor;
	fragmentColor = ambientLightStrength * vec3(texture(material.diffuse, tCoord));

	// Diffuse light
	float diffuseStrength = max(dot(normalize(-directionalLightDir), fragNormal), 0.0);
	vec3 diffuseLight = diffuseStrength * vec3(texture(material.diffuse, tCoord));

	fragmentColor += diffuseLight;

	// Specular light
	vec3 fragToEyeVector = normalize(eyePos - fragWorldPos);
	vec3 reflectedLight = normalize(reflect(directionalLightDir, fragNormal));

	float specularStrength = pow(max(dot(fragToEyeVector, reflectedLight), 0.0), material.shininess);
	vec3 specular = specularStrength * material.specular;

	fragmentColor += specular;

	return fragmentColor;
}